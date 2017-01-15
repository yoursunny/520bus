<?php
$route = $argv[1];

$table = array(); // keyword=>{stopId,dir,stopName}
$rows = file('stopnames-'.$route.'.txt');
foreach ($rows as $row) {
  list($dir, $stopId, $stopName) = str_getcsv(trim($row));
  if (!isset($table[$stopName])) {
    $table[$stopName] = array();
  }
  array_push($table[$stopName], array($stopId, $dir, $stopName));
}

echo json_encode($table);
?>
