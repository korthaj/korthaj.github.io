<?php
$namn = $_POST["namn"];
$meddelande = $_POST["meddelande"];
$foretag = $_POST["foretag"];
$telefon = $_POST["telefon"];
$epost = $_POST["epost"];

// If the substring "korthaj" occurs in the "namn" field
// or the substring "Content-Type" occurs in the "namn" field
// this is probably an attempt to use the form for mail relay.
//$pos1 = strpos($namn, "korthaj");
//$pos2 = strpos($namn, "Content-Type");
// If all fields are empty this is probably a spam attack.
//$isnotempty = ($meddelande != "" or $namn != "" or $foretag != "" or
//               $telefon != "" or $epost != "");
//if ($pos1 === false and $pos2 === false and $isnotempty)

  mail("stefan@korthaj.se",
    "Formulärmail (korthaj.se)",
    "$meddelande\n\nNamn: $namn\nFöretag: $foretag\nTelefon: $telefon\nE-post: $epost",
    "From: Korthaj <stefan@korthaj.se>");
?>
<!DOCTYPE html>
<html itemscope itemtype="http://data-vocabulary.org/Person" lang="sv">

<head>
  <meta charset="utf-8">
  <title>Tack för ditt meddelande!</title>
  <link rel="stylesheet" href="../style.css">
  <link rel="icon" type="image/png" href="../spade16.png">
  <meta name="description" content="Tack för ditt meddelande.">
  <!--[if lt IE 9]><script src="../html5.js"></script><![endif]-->
</head>

<body id="top">
  <div id="container">
    <header>
      <hgroup>
        <h1><a tabindex="1" href="..">Prof. Nilsson</a></h1>
        <h2>Korthaj och falskspelare</h2>
      </hgroup>
    </header>

    <nav>
      <ul class="menu">
        <li class="english"><a tabindex="10" href="../en/contact/" title="In English">In English</a></li>
        <li><a tabindex="2" href="..">show</a></li>
        <li><a tabindex="3" href="../texter/">fusk</a></li>
        <li class="this"><a tabindex="5" href="../kontakt/">kontakt</a></li>
      </ul>
    </nav>

    <article>
      <h1>Kontakta <br> Stefan</h1>

      <aside>
        <div class="image" style="margin-top:60px;">
          <img src="stefan-nilsson.jpg" width="140" height="140"
            alt="Stefan Nilsson" itemprop="photo">
        </div>

        <address itemprop="address" itemscope itemtype="http://data-vocabulary.org/Address">
          <em itemprop="fn">Stefan Nilsson</em><br>
            <span itemprop="street-address">Gustav&nbsp;III:s&nbsp;boulevard&nbsp;9</span><br>
            <span itemprop="postal-code">169<span class="hair-space">&nbsp;</span>72</span>&nbsp;<span itemprop="locality">Solna</span>
        </address>

        <p class="phone">
          073-598<span class="hair-space">&nbsp;</span>02<span class="hair-space">&nbsp;</span>53
        </p>

        <p class="email">
          <a tabindex="20" href=
          "mailto:stefan@korthaj.se">stefan@korthaj.se</a>
        </p>
      </aside>
          
      <h2>Tack för ditt meddelande!</h2>

      <div class="image">
        <div class="typewriter">
          <pre><?php
            PRINT "<em>Namn:</em> $namn\n";
            PRINT "<em>Företag:</em> $foretag\n";
            PRINT "<em>Telefon:</em> $telefon\n";
            PRINT "<em>E-post:</em> $epost\n\n";
            PRINT "$meddelande";?></pre>
        </div>

        <div class="caption-alt">
          Detta brev har skickats till stefan@korthaj.se.
        </div>

        <p style="clear:both;"></p>

      </div>
    </article>

    <footer>
      <p>Copyright © 2002–2014
        <a href="https://plus.google.com/100350950314702566738/about?rel=author">Stefan Nilsson</a>
        &nbsp;|&nbsp; <a href="#top">Top</a>
      </p>
    </footer>
  </div>
</body>
</html>
