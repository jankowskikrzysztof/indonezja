  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=6><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr>
     <th>Name</th>
     <th>Percentage</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT instructor.name,instructor.percentage,instructor.id_instructor
                              FROM instructor ORDER BY name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['name'].'</td>
   <td align=center>'.$row['percentage'].' %</td>
   <td><a href="?action=edit&id='.$row['id_instructor'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
