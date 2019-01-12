
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


$date_from = $_GET['date_from'];
if(!$date_from)
  $date_from = date('Y').'-'.date('m').'-01';
$date_to = $_GET['date_to'];
if(!$date_to)
  $date_to = date('Y').'-'.date('m').'-'.date('t');


include('pages/report_monthly_sale_main.inc');

?>




<?php include('inc/footer.inc'); ?>