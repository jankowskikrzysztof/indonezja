  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=5><a class="pure-button pure-button-primary" href="?period=<?php echo $period; ?>&action=add"><i class="fa fa-plus" aria-hidden="true"></i> Add</a></th>
  </tr>
  <tr>
     <th align=center>Type</th>
     <th align=center>Name</th>
     <th align=center>Limit location</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT cash_book_group.name, cash_book_group.type, cash_book_group.id_cash_book_group, REPLACE(location_id_limit,'\"','') as location_id_limit, only_cash,
                                  case cash_book_group.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                         FROM cash_book_group ORDER BY type, name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['type_name'].'</td>
   <td>'.$row['name'].'</td>';

   if($row['location_id_limit']<>0)
      {
      $pers_loc_list = '';

      $stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location, location.room_charge_perc
                              FROM location
                              WHERE location.id_location IN (".$row['location_id_limit'].")
                              ORDER BY location.name");
      $stmt -> execute();
      $location_array = $stmt->fetchAll();

      foreach($location_array as $row1)
         {
         $pers_loc_list .= $row1['loc_name'].',<br>';
         }

      echo '<td align=center>'.$pers_loc_list.'</td>';

      }
   else
       {
        echo '<td align=center>No limit</td>';
       }

   echo '
    <td><a href="?action=edit&id='.$row['id_cash_book_group'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
