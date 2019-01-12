<?php
$action = $_GET['action'];

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');

include('inc/nav.inc');

?>


  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr align=center>
     <th>file</th>
     <th>size</th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT file, size, size_type
                              FROM mega_bkp.list ORDER BY file desc");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $filename = explode('/',$row['file']);
    if($filename[2]<>'')
        $filename_show = $filename[2];
    else
        $filename_show = $filename[1];

   $file_size = round($row['file_size']/1024);

   echo '<tr>
   <td>'.$filename_show.'</td>
   <td align=right>'.str_replace('(','',$row['size']).' '.str_replace(array(',',')'),'',$row['size_type']).'</td>
   </tr>';


   }

?>
</table>


<?php include('inc/footer.inc'); ?>