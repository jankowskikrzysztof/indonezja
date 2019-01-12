  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=5><a class="pure-button pure-button-primary" href="?period=<?php echo $period; ?>&action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr>
     <th>Lp</th>
     <th>Shortcut</th>
     <th>Name</th>
     <th>Description</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT activity.* FROM activity ORDER BY id_activity");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $lp++;
   echo '<tr>
   <td>'.$lp.'</td>
   <td>'.$row['shortcut'].'</td>
   <td>'.$row['name'].'</td>
   <td>'.$row['description'].'</td>
   <td><a href="?action=edit&id='.$row['id_activity'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }


?>

</table>
