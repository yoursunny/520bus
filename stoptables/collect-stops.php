<?php
$route = $argv[1];
list($route, $dir) = explode(':', $route);
list($dir, $line) = explode(';', $dir);

$doc = new DOMDocument;
$doc->loadHTMLFile('http://infoweb.suntran.com/hiwire?.a=iHeadwaySheet&LineAbbr='.$route.'&DirectionName='.$dir.'&LineDirId='.$line);
$xml = simplexml_import_dom($doc);
$xsel = $xml->xpath('//select[@id="route-stops"]');
$xsel = $xsel[0];

foreach ($xsel->option as $xoption) {
  echo strval($xoption)."\n";
}
?>
