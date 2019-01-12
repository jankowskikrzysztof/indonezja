<?php
$action = $_GET['action'];

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);



$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');




if($action == 'xlse')
    {
    include('pages/costs_xls_export.inc');
    exit;
    }


include('inc/nav.inc');

if($action == 'add' or $action == 'edit')
include('pages/costs_form.inc');
elseif($action == 'post' or $action == 'del')
include('pages/costs_form_post.inc');
else
include('pages/costs_main.inc');

?>



<?php include('inc/footer.inc'); ?>