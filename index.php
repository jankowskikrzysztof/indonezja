<?php
header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
header("Pragma: no-cache"); // HTTP 1.0.
header("Expires: 0"); // Proxies.

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');




//session_register("personel");
//session_register("personel_location");
//$_SESSION["personel"] = 1;

include('inc/nav.inc');

include('pages/index_main.inc');

?>


<?php include('inc/footer.inc'); ?>
