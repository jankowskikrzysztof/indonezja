<?php
$action = $_GET['action'];

if($action == 'xlse')
    {
    include('pages/budget_xls_export.inc');
    exit;
    }

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
  include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');


$period_month = substr($period,5,2);
$period_year = substr($period,0,4);


if($action == 'add' or $action == 'edit')
	include('pages/budget_form.inc');
elseif($action == 'add_ba' or $action == 'edit_ba')
	include('pages/budget_form_ba.inc');
elseif($action == 'post' or $action == 'post_ba' or $action == 'del')
	include('pages/budget_form_post.inc');
else
	include('pages/budget_main.inc');

?>



<?php include('inc/footer.inc'); ?>
