<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=6><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr>
     <th>ID</th>
     <th>Name</th>
     <th>Address</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT company.*
                              FROM company 
                              ORDER BY name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['id_company'].'</td>
   <td>'.$row['name'].'</td>
   <td>'.$row['address'].'</td>
   <td><a href="?action=edit&id='.$row['id_company'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
