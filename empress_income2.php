<?php
$action = $_GET['action'];

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
           {
/*http://techstream.org/Web-Development/PHP/Dynamic-Form-Processing-with-PHP*/
           include('inc/addremoverows.js');
           include('pages/empress_income_form2.inc');
           }
elseif($action == 'del')
include('pages/empress_income_form_post2.inc');
else
include('pages/empress_income_main2.inc');

?>


<?php include('inc/footer.inc'); ?>