/*
   Revised 11 oct 1999:

      No changes have been made to the code
      only the comments have been extended.
      Also, my address has changed:

         Stefan Nilsson
         KTH, Nada
         SE-100 44 Stockholm
         Sweden

         http://www.nada.kth.se/~snilsson
*/
/*
   This is an impelementation of an integers sorting algorithm that
   sorts n word-sized integers in O(n log log n) worst-case time.

   The algorithm was originally presented in the article:

      A. Andersson, T. Hagerup, S. Nilsson, and R. Raman. Sorting
      in linear time? In Proceedings of the 27th Annual ACM
      Symposium on the Theory of Computing, pages 427-436, 1995.

   A more complete discussion of the algorithm, the implementation
   and a comparison with other well known sorting algorithms, both
   radix sorting and comparison-based methods, can be found in:

      S. Nilsson. Radix Sorting & Searching. PhD thesis, Department
      of Computer Science, Lund University, 1996.

   Both publications can be fetched from my homepage:

      http://www.cs.hut.fi/~sni/

   The code presented in this file has been tested with care but is
   not guaranteed for any purpose. The writer does not offer any
   warranties nor does he accept any liabilities with respect to
   the code.

   Stefan Nilsson, 26 oct 1996.

   Laboratory of Information Processing Science
   Helsinki University of Technology
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

#define W 32                /* Word length, machine dependent */
typedef unsigned int word;  /* W-bit word */
word Masks[5];              /* 5 = log_2(W) */

/* Utilities
*/
typedef enum {FALSE, TRUE} boolean;

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define AND   &
#define OR    |
#define XOR   ^

/* Print binary represenation of word A in chunks of b bits.
   Used only for debugging.
*/
void PrintWord(char *s, word A, int b)
{
   int i;

   printf("%s ", s);
   for (i = W - 1; i >= 0; --i) {
      printf("%1u", A >> i AND 1U);
      if (i % (b+1) == 0) printf(" ");
   }
   printf("\n");
}

/* Linked list
*/
typedef struct listrec *list;
struct listrec {
   word K;
   list next;
};

/* Each bucket contains a linked list and a pointer "check" used to
   check whether the bucket is active or not. If the bucket is active
   this pointer will point to an entry in the Active array. This
   entry will in turn have a pointer pointing back to this record.
   In this way we can check if a bucket is active without having to
   initialize the buckets.
*/
typedef struct bucketrec {
   list head, tail;
   word check;
} bucket;

/* Append the list record r to the end of list.
*/
void AddToTail(list r, list *head, list *tail)
{
   if (!*head)
      *head = r;
   else
      (*tail)->next = r;
   *tail = r;
}

/* Find leftmost nonzero bit.
*/
int Log(int n)
{
   int x = -1;

   while (n > 0) {
      n >>= 1;
      x++;
   }
   return x;
}

/* Create masks with the following testbits (k=4):

      M[0]   = "1 1 1 1 1 1 1 1"
      M[1]   = "0 0 0 0 1 1 1 1"
      M[2]   = "0 0 1 1 0 0 1 1"
      M[3]   = "0 1 0 1 0 1 0 1"

   Used with k b-bit numbers (not counting testbits).
*/
void MakeMasks(word M[], int k, int b)
{
   int s, t;
   const int log_k = Log(k);

   M[0] = 1<<b;
   s = b+1;
   for (t = 0; t <= log_k; t++, s <<= 1)
      M[0] = M[0] OR M[0]<<s;

   s = (b+1)<<log_k;  /* s = k(b+1) */
   for (t = 0; t <= log_k; t++, s >>= 1)
      M[t+1] = M[t] XOR M[t]<<s;
}


/* Change the word "1000 0000 1000" into "0111 0000 0111".
   Each word contains b-bit numbers (not counting testbits).
*/
word CopyTestBit(word A, int b)
{
   return A - (A>>b);
}

/* Merge two words containing sorted sequences
   of k b-bit numbers (not counting testbits).
*/
word WordMerge(word X, word Y, int k, int b)
{
   word Z, A, B, M, N;
   int s, t;
   const int log_k = Log(k);
   
   /* Reverse the order of the elements in word Y. */
   s = (b+1)<<log_k;  /* s = k(b+1) */
   for (t = 0; t <= log_k; t++, s >>= 1) {
      M = CopyTestBit(Masks[t+1], b);
      Y = (Y AND M)<<s OR (Y - (Y AND M))>>s;
   }
   Z = X OR Y; 

   /* Merge using the bitonic lemma. */
   s = (b+1)<<log_k; /* s = k(b+1) */
   for (t = 0; t <= log_k; t++, s >>= 1) {
      M = CopyTestBit(Masks[t+1], b);
      A = (Z AND M)<<s;
      B = Z - (Z AND M);
      N = ((B OR Masks[0]) - A) AND Masks[0];
      N = CopyTestBit(N, b);
      Z = (B AND N) OR
          (A - (A AND N)) OR
          (A AND N)>>s OR
          (B - (B AND N))>>s;
   }
   return Z;
}

/* Merge two sorted packed lists.
   Each word contains k b-bit numbers (not counting testbits).
*/
list Merge(list X, list Y, int k, int b)
{
   list head, tail, temp;
   word Z, Z1, Z2;
   const int s = (b+1)<<Log(k);  /* s = k(b+1) */

   head = NULL;
   while (Y) {
      if (!X) {
         X = Y;
         Y = NULL;
      } else {
         Z = WordMerge(X->K, Y->K, k, b);
         Z1 = Z<<(W-s)>>(W-s); /* First half */
         Z2 = Z>>s;            /* Second half */
         if (X->K < Y->K) {    /* Compare the max elements */
            X->K = Z1; Y->K = Z2;
            temp = X; X = X->next;
         } else {
            X->K = Z2; Y->K = Z1;
            temp = Y; Y = Y->next;
         }
         temp->next = NULL;
         AddToTail(temp, &head, &tail);
      }
   }
   AddToTail(X, &head, &tail);
   return head;
}

/* Mergesort for packed lists.
   Sorts the n first elements of the list.
   After the call X points to the remainder of the list.
   Each word contains k b-bit numbers (not counting testbits).
*/
list Mergesort(list *X, int n, int k, int b)
{
   list temp;

   if (!*X)
      return NULL;
   else if (n > 1)
      return Merge(Mergesort(X, (n+1)/2, k, b),
                   Mergesort(X, n/2, k, b),
                   k, b);
   else {
      temp = *X;
      *X = (*X)->next;
      temp->next = NULL;
      return temp;
   }
}

/* Construct a packed list with k b-bit keys in each word
   (not counting testbits). Returns the length of list x.
*/
list Pack(list x, int k, int b, int *n)
{
   list head, tail, temp, top;
   const int s = (b+1)<<Log(k);  /* s = k(b+1) */
   const int maxkey = CopyTestBit(1<<b, b);
   int pos;
   *n = 0;

   MakeMasks(Masks, 1, b);
   head = NULL;
   while (x) {
      /* Sort the first k elements. After the call x points
         to the remaining unsorted list. */
      top = Mergesort(&x, k, 1, b);

      /* Append a new list record. */
      temp = (list) malloc(sizeof(struct listrec));
      temp->K = 0;
      temp->next = NULL;
      AddToTail(temp, &head, &tail);

      /* Pack k elements into one word. */
      pos = 0;
      for ( ; top; top = top->next) {
         (*n)++;
         tail->K = tail->K OR top->K<<pos;
         pos += b+1;
      }
   }
   while (pos <  s) {  /* Padd the last word. */
      tail->K = tail->K OR maxkey<<pos;
      pos += b+1;
   }
   return head;
}

/* Unpack a packed list of length n.
   Each word contains k b-bit numbers (not counting testbits).
*/
list Unpack(list x, int n, int k, int b)
{
   list head, tail, temp;
   const int maxkey = CopyTestBit(1<<b, b);
   int i;

   head = NULL;
   for (i = 0; i < n; i++) {
      if (i != 0 && i % k == 0)
         x = x->next;
      temp = (list) malloc(sizeof(struct listrec));
      temp->next = NULL;
      AddToTail(temp, &head, &tail);
      tail->K = x->K AND maxkey;
      x->K >>= b+1;
   }
   return head;
}

/* Sort a list of b-bit integers using packed mergesort.
*/
list PackSort(list x, int b)
{
   list wordlist;
   int k;       /* The number of keys we can fit into a word; */
   int i;       /* that is, the largest k such that 2k(b+1) <= w */
                /* and k = 2^i for some integer i. */
   int n;       /* The length of list x */

   for (i = 1; (b+1)<<(i+1) <= W; i++)
      ;
   k = 1<<(i-1);

   wordlist = Pack(x, k, b, &n);
   MakeMasks(Masks, k, b);
   wordlist = Mergesort(&wordlist, (n+k-1)/k, k, b);
   return Unpack(wordlist, n, k, b);
}

/* Extract the b/2 least significant digits of a b-bit word.
*/
word Low(word x, int b)
{
   return x << (W - b/2) >> (W - b/2);
}

/* Extract the (b+1)/2 most significant digits of a b-bit word
 */
word High(word x, int b)
{
   return x >> (b/2);
}


/* Put a word into the batch list.
*/
void IntoBatch(word x, list *Batch)
{
   list temp;

   temp = (list) malloc(sizeof(struct listrec));
   temp->K = x;
   temp->next = *Batch;
   *Batch = temp;
}

/* Move the elements in list x into bucket B.
   The elemens are b-bit integers. The boolean hi
   indicates whether bucketing is performed on the (b+1)/2
   high order bits, or else, the b/2 low order bits.
   The Active list and the Batch is updated accordingly.
*/
void IntoBucket(list x, bucket B[],
                int b, boolean hi,
                word Active[], word *firstfree,
                list *Batch)
{
   word a, p;

   a = hi ? High(x->K, b) : Low(x->K, b);

   /* Check if bucket is nonactive. If so, add to Active. */
   p = B[a].check;
   if (p >= *firstfree || Active[p] != a) {
      B[a].head = B[a].tail = NULL;
      B[a].check = *firstfree;
      Active[(*firstfree)++] = a;
      IntoBatch(a, Batch);
   }

   /* Into bucket */
   AddToTail(x, &(B[a].head), &(B[a].tail));
}

/* Move the minimum element of bucket B[i] to the front.
   Leave this element in the bucket and return the remainder
   of the list.
*/
list Tail(bucket b[], int i)
{
   word min = ~0;
   list t;

   for (t = b[i].head; t; t = t->next)
      min = MIN(min, t->K);
   for (t = b[i].head; t->K != min; t = t->next)
      ;
   t->K = b[i].head->K;
   b[i].head->K = min;

   b[i].tail = b[i].head;
   t = b[i].head->next;
   b[i].head->next = NULL;
   return t;
}

/* Sort a list of length n. Each element is a b-bit number.
*/
list Sort(list x, int n, int b)
{
   /* Bucket arrays for high and low order bits, respectively. */
   bucket *H; /* High order bits */
   bucket *L; /* Low order bits */

   /* Keep track of what buckets are active. These arrays work
      in unison with the "check" pointer in each bucket. */
   word *Hactive; /* Array of indices to active buckets */
   word *Lactive;
   word Hfirstfree = 0;  /* Number of occupied buckets */
   word Lfirstfree = 0;

   /* The batch list. */
   list Batch = NULL;

   const boolean HIGH = TRUE, LOW = FALSE;
   word p, i;
   list next, s, t, head, tail;

   H = (bucket *) malloc((1<<((b+1)/2)) * sizeof(struct bucketrec));
   Hactive = (word *) malloc(n * sizeof(word));
   L = (bucket *) malloc((1<<(b/2)) * sizeof(struct bucketrec));
   Lactive = (word *) malloc(n * sizeof(word));

   /* Bucketing on high order bits */
   for ( ; x; x = next) {
      next = x->next;
      x->next = NULL;
      IntoBucket(x, H, b, HIGH, Hactive, &Hfirstfree, &Batch);
   }

   /* Bucketing on low order bits */
   /* The minimum element of each H-bucket is left behind */
   for (i = 0; i < Hfirstfree; i++) {
      s = Tail(H, Hactive[i]);
      for ( ; s; s = next) {
         next = s->next;
         s->next = NULL;
         IntoBucket(s, L, b, LOW, Lactive, &Lfirstfree, &Batch);
      }
   }

   /* Sort the batch list with apropriate sorting algorithm. */
   if ((b/2)<<Log(Log(n)) >= W && b >= 4)
      Batch = Sort(Batch, n, (b+1)/2);
   else
      Batch = PackSort(Batch, (b+1)/2);

   /* Move the elements back to the high order bucket table */
   for (s = Batch; s; s = s->next) {
      p = L[s->K].check;
      if (p < Lfirstfree && Lactive[p] == s->K) {
         for (t = L[s->K].head; t; t = next) {
            next = t->next;
            t->next = NULL;
            H[High(t->K, b)].tail->next = t;
            H[High(t->K, b)].tail = t;
         }
         L[s->K].head = NULL;
      }
   }

   /* Traverse the buckets and assemble the output. */
   head = tail = NULL;
   for (s = Batch; s; s = s->next) {
      p = H[s->K].check;
      if (p < Hfirstfree && Hactive[p] == s->K) {
         if (!head)
            head = H[s->K].head;
         else
            tail->next = H[s->K].head;
         tail = H[s->K].tail;
         H[s->K].head = NULL;
      }
   }

   return head;
}

int main(int argc, char *argv[])
{
   int n = 0;   /* Number of elements */
   word x;
   list head, tail, temp;
   clock_t startclock, stopclock;
   FILE *in_file;

   if (argc != 2) {
      fprintf(stderr, "%s%s%s\n", "Usage: ", argv[0], " file_name");
      return 1;
   }
   if (!(in_file = fopen(argv[1], "rb"))) {
      perror(argv[0]);
      return 1;
   }

   head = NULL;
   while (fscanf(in_file, "%i", &x) != EOF) {
      n++;
      temp = (list) malloc(sizeof(struct listrec));
      temp->K = x;
      temp->next = NULL;
      AddToTail(temp, &head, &tail);
   }

   startclock = clock();
   head = Sort(head, n, W);
   stopclock = clock();

   for ( ; head; head = head->next)
      printf("%i\n", head->K);

   fprintf(stderr, "%s%.2f%s\n", "Time: ",
            (stopclock-startclock) / (float) CLOCKS_PER_SEC,
            " sec.");
   return 0;
}

