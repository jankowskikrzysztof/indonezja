<?php
$action = $_GET['action'];

$header_title = 'Scuba -> Clearance Lists';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
   include('inc/nav.inc');

if($action == 'edit')
   include('pages/clearance_form.inc');
elseif($action == 'post' or $action == 'del')
   include('pages/clearance_form_post.inc');
else
   include('pages/clearance_main.inc');
?>


<?php include('inc/footer.inc'); ?>