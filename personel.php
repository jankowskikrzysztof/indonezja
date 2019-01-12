<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
   include('inc/nav.inc');

if($action == 'add' or $action == 'edit')
include('pages/personel_form.inc');
elseif($action == 'post' or $action == 'del')
include('pages/personel_form_post.inc');
else
include('pages/personel_main.inc');
?>


<?php include('inc/footer.inc'); ?>