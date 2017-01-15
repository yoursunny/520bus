<?php

$table = array(); // dir=>stopId=>stopName
for ($i = 1; $i < $argc; ++$i) {
  $filename = $argv[$i];
  $dir = explode('-', $filename);
  $dir = str_replace('.txt', '', $dir[2]);
  $table[$dir] = array();
  $rows = file($filename);
  foreach ($rows as $row) {
    list($stopId, $stopName) = explode(' -- ', trim($row));
    $table[$dir][$stopId] = $stopName;
  }
}

$stopNames = array(); // stopNames=>1
foreach ($table as $dir=>$dirStops) {
  foreach ($dirStops as $stopId=>$stopName) {
    $stopNames[$stopName] = 1;
  }
}

function shortenStopName($longName) {
  $s = explode('/', $longName);
  if (count($s) == 3 && $s[0] == 'SL') {
    return $s[2];
  }
  if (count($s) == 2) {
    return $s[1];
  }

  $s = explode('@', $longName);
  if (count($s) == 2) {
    return $s[0];
  }

  $s = explode(' ', $longName);
  if (count($s) == 3 && strtolower($s[2]) == 'center') {
    return $s[0];
  }

  return str_replace('/', ' and ', $longName);
}

$shortStopNames = array(); // shortName=>longName=>1
foreach ($stopNames as $stopName=>$ignored) {
  $shortName = shortenStopName($stopName);
  $shortStopNames[$shortName][$stopName] = 1;
}

$shortenMap = array(); // longName=>shortName
foreach ($shortStopNames as $shortName=>$longNames) {
  $longNames = array_keys($longNames);
  if (count($longNames) == 1 || strpos($longNames[0], '@') !== FALSE || substr($longNames[0], 0, 3) == 'SL/') {
    foreach ($longNames as $longName) {
      $shortenMap[$longName] = $shortName;
    }
  }
  else {
    foreach ($longNames as $longName) {
      $shortenMap[$longName] = str_replace('/', ' and ', $longName);
    }
  }
}

foreach ($table as $dir=>$dirStops) {
  foreach ($dirStops as $stopId=>$stopName) {
    echo $dir.','.$stopId.',"'.$shortenMap[$stopName].'"'."\n";
  }
}

?>
