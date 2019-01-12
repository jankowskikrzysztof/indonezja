  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
     <th>Name</th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT location_groups.name as group_name FROM location_groups ORDER BY group_name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['group_name'].'</td>
   </tr>';


   }

?>
</table>
