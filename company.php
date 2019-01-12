<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
   include('inc/nav.inc');

if($action == 'add' or $action == 'edit')
include('pages/company_form.inc');
elseif($action == 'post' or $action == 'del')
include('pages/company_form_post.inc');
else
include('pages/company_main.inc');
?>


<?php include('inc/footer.inc'); ?>