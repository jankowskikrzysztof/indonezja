
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
if($action=='cc')
include('pages/report_group_settlements_cc.inc');
elseif($action=='bookers')
include('pages/report_group_settlements_bookers.inc');


?>




<?php include('inc/footer.inc'); ?>