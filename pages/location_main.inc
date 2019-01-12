  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
     <th>Group</th>
     <th>Name</th>
     <th>Room Charge</th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT location.name, location_groups.name as group_name, location.room_charge_perc FROM `location`, location_groups WHERE location.location_groups_id=location_groups.id_location_groups ORDER BY group_name, name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['group_name'].'</td>
   <td>'.$row['name'].'</td>
   <td align=center>'.$row['room_charge_perc'].' %</td>
   </tr>';


   }

?>
</table>
