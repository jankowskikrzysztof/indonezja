<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');




//session_register("personel");
//session_register("personel_location");
$_SESSION["personel"] = 1;

include('inc/nav.inc');

include('pages/index_main.inc');

?>

test

<?php include('inc/footer.inc'); ?>