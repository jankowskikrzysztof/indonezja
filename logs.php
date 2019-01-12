<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
  include('inc/nav.inc');


if($action == 'main')
include('pages/logs_main.inc');
elseif($action == 'table')
include('pages/logs_table.inc');
elseif($action == 'last_changes')
   {
   $period = $_GET['period'];
   if(!$period)
       $period = date('Y').'-'.date('m');

   $period_month = substr($period,5,2);
   $period_year = substr($period,0,4);


   include('pages/logs_last_changes.inc');
   }

?>


<?php include('inc/footer.inc'); ?>