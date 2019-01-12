      <style>
         tr {
             font-size: 100%;
         }
      </style>

<script>
function changeTrip(url)
{
    location.href = 'empress_guests.php?trip_id=' + url;
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
  <tr align=center>
     <th></th>
     <th>Name</th>
     <th>Trip</th>
     <th>Bill no</th>
     <th>Date of born</th>
     <th>Gender</th>
     <th>Nationality</th>
     <th>Nitrox</th>
     <th>Remarks</th>
     <th>email</th>

     <th></th>
  </tr>
 </thead>

<?php

$where_trip = '';
if($_GET['trip_id']<>0)
   $where_trip = 'and cash_book.boat_trip_id='.$_GET['trip_id'];

/*
$stmt = $dbh->prepare("SELECT client.*,
cash_book.id_cash_book,
cash_book.bill_no,
booker.name as booker_name,
boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route,
activity.name as act_name, activity.shortcut as act_shortcut

                              FROM client
                              left join cash_book on cash_book.client_id=client.id_client
                              left join cash_book_item on cash_book_item.cash_book_id=cash_book.id_cash_book
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join activity on activity.id_activity=cash_book_item.activity_id
                              ".$where_trip."
                              GROUP BY client.id_client
                              ORDER BY act_shortcut, name");
*/

                              $stmt = $dbh->prepare("SELECT client.*,
                              cash_book.id_cash_book,
                              cash_book.bill_no,
                              booker.name as booker_name,
                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route,
                              activity.name as act_name, activity.shortcut as act_shortcut
                              
                                                            FROM 
                                                            (cash_book,cash_book_item,boat_trip,booker,activity)
                                                            
                                                            left join client on cash_book.client_id=client.id_client

                                                            WHERE cash_book_item.cash_book_id=cash_book.id_cash_book
                                                            and boat_trip.id_boat_trip=cash_book.boat_trip_id
                                                            and cash_book.booker_id=booker.id_booker
                                                            and activity.id_activity=cash_book_item.activity_id
                                                            and cash_book.type_cost=0
                                                            ".$where_trip."
                                                            ORDER BY act_shortcut, name");
                              

                              $stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {


//<br>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'


   if($last_cabin<>substr($row['act_shortcut'],0,1))
    {
        echo '<tr>
                <td colspan=15><h2>'.substr($row['act_name'],0,7).'</h2></td>
              </tr>';
    }


   echo '<tr>
   <td>'.substr($row['act_name'],8,5).'</td>
   <td>'.$row['name'].'</td>
   <td>'.$row['booker_name'].'</td>
   <td align=right><a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'">invoice: '.$row['bill_no'].'</a></td>
   <td>'.$row['date_of_born'].'</td>
   <td>'.$row['gender'].'</td>
   <td>'.$row['nationality'].'</td>
   <td>'.$row['buyer'].'</td>
   <td>'.$row['nitrox'].'</td>
   <td>'.$row['remarks'].'</td>
   <td><a href="mailto:'.$row['email'].'">'.$row['email'].'</a></td>
   </tr>';

   $last_cabin = substr($row['act_shortcut'],0,1);
   }

?>
</table>
