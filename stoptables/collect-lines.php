<?php
$doc = new DOMDocument;
$doc->loadHTMLFile('http://infoweb.suntran.com/hiwire?.a=iScheduleLookup');
$xml = simplexml_import_dom($doc);
$xsel = $xml->xpath('//select[@id="LineSelect"]');
$xsel = $xsel[0];

foreach ($xsel->option as $xoption) {
  echo $xoption['value']."\n";
}
?>
