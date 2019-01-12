
<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);

$action = $_GET['action'];

if($action=='main')
  include('pages/report_main.inc');

if($action=='room_charge')
  include('pages/report_room_charge.inc');

if($action=='empress_trips')
  include('pages/report_empress_trips.inc');  

?>




<?php include('inc/footer.inc'); ?>