      <style>
         tr {
             font-size: 80%;
         }
      </style>

  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=100%><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
  </tr>
  <tr align=center>
     <th>Name</th>
     <th>Login</th>
     <th>Lastlog</th>
     <th>Have password</th>
     <th>Limit location</th>
     <th>Permission</th>
     <th>Limit Data Months</th>
     <th>Limit site</th>
     <th>Timezone</th>
     <th></th>
  </tr>
 </thead>

<?php



$stmt = $dbh->prepare("SELECT personel.name,personel.surname,personel.email, personel.login, personel.id_personel, personel.location_id_limit, personel.permission, personel.limit_site, personel.timezone,personel.limit_data_months,
                              if(password is null OR password = '' OR password = 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855', 'NO - blocked', 'YES') as is_password,
                             MAX(logs.insertdate) as last_log
                              FROM personel
                              left join logs on personel.id_personel=logs.personel_id and logs.type='login'

                              GROUP BY personel.id_personel
                              ORDER BY is_password desc, name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $last_log = explode(' ',$row['last_log']);

   if($last_log[0] == date("Y-m-d"))
      $bg_last_color = ' style="background-color:#98FB98"';
   else
      $bg_last_color = '';

   if($row['limit_data_months']==0)
      $bg_limit_data_color = ' style="background-color:#FFD000"';
   else
      $bg_limit_data_color = '';

   if($row['is_password']<>'YES')
      $bg_pers_block_color = ' style="background-color:#FFCCCB"';
   else
      $bg_pers_block_color = '';


   echo '<tr>
   <td>'.$row['name'].'</td>
   <td>'.$row['login'].'</td>
   <td align=center'.$bg_last_color.'><a href="logs.php?action=main&id='.$row['id_personel'].'">'.$row['last_log'].'</a></td>
   <td align=center'.$bg_pers_block_color.'>'.$row['is_password'].'</td>';


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
   <td align=center>'.$row['permission'].'</td>
   <td align=center'.$bg_limit_data_color.'>'.$row['limit_data_months'].'</td>
   <td align=center>'.$row['limit_site'].'</td>
   <td align=center>'.$row['timezone'].'</td>
   <td><a href="?action=edit&id='.$row['id_personel'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
<tr><td colspan=100%>Permission legend:<br>rw - read/write access<br>ro - read only</td></tr>

</table>
