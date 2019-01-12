  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=5><a class="pure-button pure-button-primary" href="?period=<?php echo $period; ?>&action=add"><i class="fa fa-plus" aria-hidden="true"></i> Add</a></th>
  </tr>
  <tr>
     <th>Activity</th>
     <th>Location group</th>
     <th>Price</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT activity.name,activity_price_list.price, activity_price_list.id_activity_price_list, location_groups.name as loc_name
                              FROM activity, activity_price_list, location_groups
                              WHERE activity.id_activity=activity_price_list.activity_id
                              and location_groups.id_location_groups=activity_price_list.location_groups_id
                              ORDER BY loc_name,activity.name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['name'].'</td>
   <td>'.$row['loc_name'].'</td>
   <td align=right>'.$row['price'].'</td>
   <td><a href="?action=edit&id='.$row['id_activity_price_list'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }


?>

</table>
