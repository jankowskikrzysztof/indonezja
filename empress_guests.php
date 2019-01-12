<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
   include('inc/nav.inc');

   include('pages/empress_guests_main.inc');
?>


<?php include('inc/footer.inc'); ?>