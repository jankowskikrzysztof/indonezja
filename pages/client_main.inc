      <style>
         tr {
             font-size: 70%;
         }
      </style>

<script>
function changeTrip(url)
{
    location.href = 'client.php?trip_id=' + url;
}
</script>

<?php
$stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, id_boat_trip as boat_trip_id
                              FROM boat_trip
                              ORDER BY date_to");
$stmt -> execute();
$boat_trip_array = $stmt->fetchAll();
$date_now = date("Y-m-d");
?>

<div align=center class="pure-form">

                <select name="trip_id" id="boat_trip_id" class="pure-input-1-4" onchange="changeTrip(this.value)">
                        <option value="0">no trip specified</option>
                <?php
                     foreach($boat_trip_array as $row)
                        {
                        //koloruje aktualny trip
                        $option_bgcolor = '';
                        if($row['date_from']<$date_now)
                           $option_bgcolor = ' style="background-color:#e6e6e6"';
                        if($row['date_from']<$date_now and $row['date_to']>$date_now)
                           $option_bgcolor = ' style="background-color:#73e600"';

                        if( $_GET['trip_id'] == $row['boat_trip_id'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['boat_trip_id'].'" '.$sel.$option_bgcolor.'>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</option>';
                        }
                ?>
                </select>
`


<br><br>
</div>


  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
      <th colspan=9><a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
      <th colspan=5 align=right>

                    <?php
                       if($_GET['trip_id']<>0)
                          { ?>
                    <form action="test_email.php" method="POST">
                    <input type="hidden" name="sel_trip" value="<?php echo $_GET['trip_id'] ?>" >
                    email: <input type=text name=email>
                    <button type="submit" class="pure-button pure-button-primary">send list</button>
                    </form>
                    <?php } ?>
      </th>
  </tr>
  <tr>
     <th>Name</th>
     <th>Trip</th>
     <th>Date of born</th>
     <th>Gender</th>
     <th>Nationality</th>
     <th>No of dives</th>
     <th>Passport</th>
     <th>Buyer</th>
     <th>Nitrox</th>
     <th>Gear to rent</th>
     <th>Remarks</th>
     <th>email</th>
     <th>Park fee</th>

     <th></th>
  </tr>
 </thead>

<?php

$where_trip = '';
if($_GET['trip_id']<>0)
   $where_trip = 'WHERE cash_book.boat_trip_id='.$_GET['trip_id'];

$stmt = $dbh->prepare("SELECT client.*,
cash_book.id_cash_book,
booker.name as booker_name,
boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route

                              FROM client
                              left join cash_book on cash_book.client_id=client.id_client
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              ".$where_trip."
                              GROUP BY client.id_client
                              ORDER BY name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

   if($row['id_cash_book']<>'')
      $trip = '<a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['booker_name'].'<br>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</a>';
   else
      $trip = '';

   echo '<tr>
   <td>'.$row['name'].'</td>
   <td>'.$trip.'</td>
   <td>'.$row['date_of_born'].'</td>
   <td>'.$row['gender'].'</td>
   <td>'.$row['nationality'].'</td>
   <td>'.$row['no_of_dives'].'</td>
   <td>'.$row['passport'].'</td>
   <td>'.$row['buyer'].'</td>
   <td>'.$row['nitrox'].'</td>
   <td>'.$row['gear_to_rent'].'</td>
   <td>'.$row['remarks'].'</td>
   <td><a href="mailto:'.$row['email'].'">'.$row['email'].'</a></td>
   <td>'.$row['park_fee'].'</td>

   <td><a href="?action=edit&id='.$row['id_client'].'"><i class="far fa-edit" aria-hidden="true"></i></a></td>
   </tr>';


   }

?>
</table>
