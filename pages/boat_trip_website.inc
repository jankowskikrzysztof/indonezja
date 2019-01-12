<script src="https://unpkg.com/sticky-table-headers"></script>

<center><br>
<img src="http://empress.famapr.pl/wp-content/uploads/2016/01/logo100.png">

<h1>Dive Safari Schedule for EMPRESS</h1></center>

  <table class="pure-table pure-table-bordered">
   <thead>
  <tr>
     <th align=center>Dates / Route / Duration</th>
     <th></th>
     <th align=center>CABIN A<br>price for 1 bed<br>(MAX 3 PERSON, SINGLE BEDS)</th>
     <th align=center>CABIN B<br>price for 1 bed<br>(MAX 3 PERSONS: 2+1)</th>
     <th align=center>CABIN C<br>price for 1 bed<br>(MAX 3 PERSONS: 2+1)</th>
     <th align=center>CABIN D<br>price for 1 bed<br>(MAX 3 PERSONS: 2+1)</th>
     <th align=center>CABIN E<br>PRICE FOR 1 BED<br>(MAX 1 PERSON)</th>
  </tr>
 </thead>

<?php

function show_beds($total_beds=0,$occupy_beds=0)
         {
         $grey_beds = $occupy_beds;
         $green_beds = $total_beds - $occupy_beds;
         $show_beds = '';

         for($x=1;$x<=$grey_beds;$x++)
            $show_beds .= '<i class="fas fa-bed fa-3x" style="color:Lightgrey"></i> ';

         for($x=1;$x<=$green_beds;$x++)
            $show_beds .= '<i class="fas fa-bed fa-3x" style="color:Green"></i> ';

         return $show_beds;
         }


$date_now = date("Y-m-d");

setlocale(LC_MONETARY,'id_ID');

$select_cabs = 'SELECT
count(id_activity) as beds,
name,
tech_cab_num
FROM `activity`
WHERE tech_cab_num<>0
GROUP BY tech_cab_num';

$stmt = $dbh->prepare($select_cabs);

$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $cab_beds[$row['tech_cab_num']] =  $row['beds'];
   }


if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

//cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,


$select = "SELECT

GROUP_CONCAT(activity.shortcut) as shortcut,
`cash_book`.`id_cash_book`,
`cash_book`.`boat_trip_id`,
activity.tech_cab_num,
`cash_book`.value as cb_value,
SUM(`cash_book_item`.value) as item_value,
COUNT(`cash_book_item`.id_cash_book_item) as beds_sell,
SUM(cash_book.pay_cash)+SUM(cash_book.pay_creditcard)+SUM(cash_book.pay_roomcharge)+SUM(cash_book.pay_bank)+SUM(cash_book.pay_paypal) as total_pay,
GROUP_CONCAT(DISTINCT CONCAT(`cash_book`.bill_no,'][',cash_book.id_cash_book)) as bill_no,
GROUP_CONCAT(DISTINCT cash_book.booking_no) as booking_no,
`cash_book`.booker_perc,
`cash_book`.value_before_discount,
GROUP_CONCAT(DISTINCT booker.name) as booker_name,
GROUP_CONCAT(DISTINCT CONCAT(client.name,'][',client.id_client) SEPARATOR '[]') as client_name


FROM (`cash_book`,cash_book_item,activity)
left join booker on cash_book.booker_id=booker.id_booker
left join client on client.id_client=cash_book.client_id

WHERE `cash_book`.`boat_trip_id`<>0
and `cash_book`.`type`=1



and cash_book.location_id=14



and activity.id_activity=cash_book_item.activity_id
 and cash_book_item.cash_book_id=cash_book.id_cash_book

GROUP BY `cash_book`.boat_trip_id, activity.tech_cab_num";

//echo $select;

$stmt = $dbh->prepare($select);

$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $trip_details_booker_perc[$row['boat_trip_id']] = $row['booker_perc'];
   $trip_details_cb_value[$row['boat_trip_id']] = $row['cb_value'];

   $trip_details_total_pay[$row['boat_trip_id']][$row['id_cash_book']] = $row['total_pay'];

//   echo $row['total_pay'].' | '.$row['boat_trip_id'].' | '.$row['id_cash_book'].' | '.$row['shortcut'].'<br>';

   $trip_details_before_disc[$row['boat_trip_id']] = $row['value_before_discount'];

   $trip_details_item_value[$row['boat_trip_id']][$row['tech_cab_num']] += $row['item_value']-$row['item_value']*$row['booker_perc']/100;

   $trip_details_cabin_pay[$row['boat_trip_id']][$row['tech_cab_num']] = array_sum($trip_details_total_pay[$row['boat_trip_id']]) - array_sum($trip_details_cabin_pay[$row['boat_trip_id']]);
//   $trip_details_cabin_pay[$row['boat_trip_id']][$row['tech_cab_num']] = $trip_details_total_pay[$row['boat_trip_id']].' | '.array_sum($trip_details_cabin_pay[$row['boat_trip_id']]);

   // jezeli zaplata jest wieksza od wartosci kabiny to pokazuje tylko zaplate do wartosci kabiny
   if($trip_details_cabin_pay[$row['boat_trip_id']][$row['tech_cab_num']] > $trip_details_item_value[$row['boat_trip_id']][$row['tech_cab_num']])
      $trip_details_cabin_pay[$row['boat_trip_id']][$row['tech_cab_num']] = $trip_details_item_value[$row['boat_trip_id']][$row['tech_cab_num']];

   $trip_beds_sell[$row['boat_trip_id']][$row['tech_cab_num']] = $row['beds_sell'];
   $trip_details_booking_no[$row['boat_trip_id']][$row['tech_cab_num']] = $row['booking_no'];
   $trip_details_booker_name[$row['boat_trip_id']][$row['tech_cab_num']] = $row['booker_name'];
   $trip_details_cb_shortcut[$row['boat_trip_id']][$row['tech_cab_num']] = $row['shortcut'];

   $bill_data = explode(",",$row['bill_no']);
   foreach($bill_data as $bill_dat)
      {
      $b++;
      $bill_da = explode("][",$bill_dat);

      $trip_details_bill_no[$row['boat_trip_id']][$row['tech_cab_num']][$b] = '<a href="empress_income.php?action=edit&id='.$bill_da[1].'">'.$bill_da[0].$invoice_still_to_pay.'</a> ';
      }


   $client_name = explode("[]",$row['client_name']);
   foreach($client_name as $client_nam)
      {
      $c++;
      $client_na = explode("][",$client_nam);
      $trip_details_client_name[$row['boat_trip_id']][$row['tech_cab_num']][$c] = '<a href="client.php?action=edit&id='.$client_na[1].'">'.$client_na[0].'</a> ';
      }

   }

//echo '<pre>';
// var_dump($trip_details_bill_no);
//echo '</pre>';

$select = "SELECT boat_trip.id_boat_trip,
                              boat_trip.date_from, boat_trip.date_to,
                              boat_trip.price_cabin_a, boat_trip.txt_cabin_a,
                              boat_trip.price_cabin_b, boat_trip.txt_cabin_b,
                              boat_trip.price_cabin_c, boat_trip.txt_cabin_c,
                              boat_trip.price_cabin_d, boat_trip.txt_cabin_d,
                              boat_trip.price_cabin_e, boat_trip.txt_cabin_e,

                              boat_trip.txt_route

                              FROM boat_trip
                              WHERE
                                  `boat_trip`.`date_from` >= '".date("Y-m-d")."'

                              ORDER BY boat_trip.date_to asc";
//echo $select;

$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

   //$still_to_pay = $row['price_total']-$row['price_payed_total'];

   $still_to_pay[$row['id_boat_trip']] = array_sum($trip_details_item_value[$row['id_boat_trip']])-array_sum($trip_details_total_pay[$row['id_boat_trip']]);

   $date1 = new DateTime($row['date_from']);
   $date2 = new DateTime($row['date_to']);
   $numberOfNights = $date2->diff($date1)->format("%a");
   $numberOfDays = $numberOfNights+1;


   echo '<tr>
   <td align=center rowspan=3 '.$calendar_bgcolor.'>'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'<br>days: '.$numberOfDays.' / nights: '.$numberOfNights.'
                    ';
                echo '
                    </td>
   <td align=right>Price</td>

   <td class=cash width=250em>'.$formatter->formatCurrency($row['price_cabin_a'], 'USD').'</td>

   <td class=cash width=250em>'.$formatter->formatCurrency($row['price_cabin_b'], 'USD').'</td>

   <td class=cash width=250em>'.$formatter->formatCurrency($row['price_cabin_c'], 'USD').'</td>

   <td class=cash width=250em>'.$formatter->formatCurrency($row['price_cabin_d'], 'USD').'</td>

   <td class=cash width=250em>'.$formatter->formatCurrency($row['price_cabin_e'], 'USD').'</td>

   </tr>';

   echo '<tr class="reports pure-table">
             <td align=right>PLACES<br>available:</td>
             <td align=center>';
               if($cab_beds['1'] > $trip_beds_sell[$row['id_boat_trip']]['1'])
                 echo show_beds($cab_beds['1'],$trip_beds_sell[$row['id_boat_trip']]['1']);
               else
                 echo '<p>Sold Out</p>';
   echo '
             </td>
             <td align=center>';
               if($cab_beds['2'] > $trip_beds_sell[$row['id_boat_trip']]['2'])
                 echo show_beds($cab_beds['2'],$trip_beds_sell[$row['id_boat_trip']]['2']);
               else
                 echo '<p>Sold Out</p>';
   echo '
             </td>
             <td align=center>';
               if($cab_beds['3'] > $trip_beds_sell[$row['id_boat_trip']]['3'])
                 echo show_beds($cab_beds['3'],$trip_beds_sell[$row['id_boat_trip']]['3']);
               else
                 echo '<p>Sold Out</p>';
   echo '
             </td>
             <td align=center>';
               if($cab_beds['4'] > $trip_beds_sell[$row['id_boat_trip']]['4'])
                 echo show_beds($cab_beds['4'],$trip_beds_sell[$row['id_boat_trip']]['4']);
               else
                 echo '<p>Sold Out</p>';
   echo '
             </td>
             <td align=center>';
               if($cab_beds['5'] > $trip_beds_sell[$row['id_boat_trip']]['5'])
                 echo show_beds($cab_beds['5'],$trip_beds_sell[$row['id_boat_trip']]['5']);
               else
                 echo '<p>Sold Out</p>';
   echo '
             </td> ';


      echo '
   </tr>';

   echo '<tr class="reports pure-table-odd">
             <td align=right>Notes</td>
             <td>'.$row['txt_cabin_a'].'</td>
             <td>'.$row['txt_cabin_b'].'</td>
             <td>'.$row['txt_cabin_c'].'</td>
             <td>'.$row['txt_cabin_d'].'</td>
             <td>'.$row['txt_cabin_e'].'</td>';


      echo '
   </tr>';

   }


?>

</table>

  <script>
    $('table').stickyTableHeaders();
  </script>