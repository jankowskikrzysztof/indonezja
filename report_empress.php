<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');


$period = $_GET['period'];
$period_year = $period;

$action = $_GET['action'];


if($action == 'trip_result')
   include('pages/report_empress_trip_result.inc');

if($action == 'trip_income')
   include('pages/report_empress_trip_income.inc');

if($action == 'bookers')
   include('pages/report_empress_bookers.inc');

?>


<?php include('inc/footer.inc'); ?>