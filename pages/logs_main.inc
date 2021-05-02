  <?php

if (!isset ($_GET['page']) ) {  
   $page = 1;  
} else {  
   $page = $_GET['page'];  
}  

$select = "SELECT COUNT(id_logs) as records
                              FROM `logs`
                              WHERE logs.personel_id= ".$_GET['id']."";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row = $stmt->fetch();

$results_per_page = 500;
$pages_count = ceil($row['records']/$results_per_page);
$page_first_result = ($page-1) * $results_per_page;  

$actual_records_max = $page_first_result+$results_per_page;

echo '<br><p>Total records '.$row['records'].', Actual records: '.$page_first_result.'-'.$actual_records_max.'. Choose log page: ';

for($page = 1; $page<= $pages_count; $page++) {  
   echo '<a href = "?action='.$action.'&id='.$id.'&page=' . $page . '">' . $page . ' </a> &nbsp;&nbsp;&nbsp;';  
}

?>
  </p><br>
  
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
                              LIMIT " . $page_first_result . ',' . $results_per_page; 

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
