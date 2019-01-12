  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr align=center>
     <th>Currency</th>
     <th>Date</th>
     <th>Rate</th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT * FROM `cur_rate` ORDER BY currency, date desc");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['currency'].'</td>
   <td>'.$row['date'].'</td>
   <td class=cash>'.number_format($row['value'],4,'.',',').' IDR</td>
   </tr>';


   }

?>
</table>
