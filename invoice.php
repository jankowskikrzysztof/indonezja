<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
$period_year = $period;

$action = $_GET['action'];

if($action == 'emp')
include('pages/invoice_main.inc');
elseif($action == 'emp2')
include('pages/invoice_main2.inc');

?>


<?php include('inc/footer.inc'); ?>