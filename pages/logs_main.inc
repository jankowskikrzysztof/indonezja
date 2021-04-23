  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr class=reports>
     <th>Lp</th>
     <th>Timestamp</th>
     <th>IP</th>
     <th>Type</th>
     <th>Target table</th>
     <th>Target ID</th>
     <th>TXT</th>
  </tr>
 </thead>

<?php



$select = "SELECT logs.*
                              FROM `logs`
                              WHERE logs.personel_id= ".$_GET['id']."
                              ORDER BY insertdate desc
                              LIMIT 1000";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $lp++;

   if($row['type'] == 'del')
     $bg_type = '<font color=red>';
   else
     $bg_type = '<font>';


   echo '<tr class=reports>
   <td>'.$lp.'</td>
   <td>'.$row['insertdate'].'</td>
   <td>'.$row['ip'].'</td>
   <td>'.$bg_type.$row['type'].'</font></td>';

   if($row['target_table']=='cash_book' and $row['target_id']<>0)
      {
      echo '<td><a href="logs.php?action=table&table='.$row['target_table'].'&id='.$row['target_id'].'">'.$row['target_table'].'</a></td>
         <td><a href="logs.php?action=table&table='.$row['target_table'].'&id='.$row['target_id'].'">'.$row['target_id'].'</a></td>';
      }
   else
      {
      echo '<td>'.$row['target_table'].'</td>
         <td>'.$row['target_id'].'</td>';
      }

   echo '
   <td>'.$row['txt'].'</td>
   </tr>';


   }

?>
</table>
