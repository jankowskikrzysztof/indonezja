<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);

if($action == 'add' or $action == 'edit')
include('pages/cash_report_form.inc');
elseif($action == 'post' or $action == 'del')
include('pages/cash_report_form_post.inc');
else
include('pages/cash_report_main.inc');

?>




<?php include('inc/footer.inc'); ?>