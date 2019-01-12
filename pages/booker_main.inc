  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=6><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr>
     <th>Name</th>
     <th>Commission</th>
     <th>Company name</th>
     <th>Company address</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT booker.name,booker.commission,booker.id_booker, company.name as company_name, company.address as company_address
                              FROM booker 
                              left join company on booker.company_id=company.id_company
                              ORDER BY booker.name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['name'].'</td>
   <td align=center>'.$row['commission'].' %</td>
   <td>'.$row['company_name'].'</td>
   <td>'.$row['company_address'].'</td>
   <td><a href="?action=edit&id='.$row['id_booker'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
