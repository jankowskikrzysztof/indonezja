


  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
     <th>Trip</th>
     <th></th>
  </tr>
 </thead>

<?php

$stmt = $dbh->prepare("SELECT boat_trip.*

                              FROM boat_trip
                              ORDER BY date_to");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

   //koloruje aktualny trip
   $option_bgcolor = '';
   if($row['date_from']<date("Y-m-d"))
      $option_bgcolor = ' style="background-color:#e6e6e6"';
   if($row['date_from']<date("Y-m-d") and $row['date_to']>date("Y-m-d"))
      $option_bgcolor = ' style="background-color:#73e600"';

   echo '<tr>
     <td align=center '.$option_bgcolor.'>'.$row['date_from'].' - '.$row['date_to'].'<br><font size=0.5vw>'.$row['txt_route'].'</font></td>
   <td><a href="?action=edit&id='.$row['id_client'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
