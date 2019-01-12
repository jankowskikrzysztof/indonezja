<?php
$action = $_GET['action'];

$header_title = 'Cash Book Groups';
include('inc/header.inc');

include('inc/config.inc');

if($action <> 'post' and $action <> 'del')
  include('inc/nav.inc');


if($action == 'add' or $action == 'edit')
include('pages/cash_book_groups_form.inc');
elseif($action == 'post' or $action == 'del')
include('pages/cash_book_groups_form_post.inc');
else
include('pages/cash_book_groups_main.inc');

?>



<?php include('inc/footer.inc'); ?>