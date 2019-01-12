<script src="https://unpkg.com/sticky-table-headers"></script>

<script>
function changePeriod(url)
{
    location.href = 'boat_trip.php?period=' + url;
}
</script>


<div align=center class="pure-form">

<input type=year value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

  <table class="pure-table pure-table-bordered" width=1500em>
   <thead>
  <tr>
      <th colspan=5>
<?php
if($_SESSION['permission']=='rw')
echo '  
<a class="pure-button pure-button-primary" href="?action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd Trip</a>';
?>
      </th>
   <th align=right colspan=3><a href="https://scuba.gruvi.pl/empress_web_calendar.php">Public Calendar</a></th>
  </tr>
  <tr>
     <th align=center>Dates / Route / Duration</th>
     <th></th>
     <th align=center>Cabin A</th>
     <th align=center>Cabin B</th>
     <th align=center>Cabin C</th>
     <th align=center>Cabin D</th>
     <th align=center>Cabin E</th>
     <th align=center>Still to pay</th>
  </tr>
 </thead>

<?php


function trip_cash_payed($trip_id,$dbh)
         {
         $stmt = $dbh->prepare("SELECT
                                       SUM(cash_book.value) as value,
                                       SUM(cash_book.pay_cash)+SUM(cash_book.pay_creditcard)+SUM(cash_book.pay_roomcharge)+SUM(cash_book.pay_bank)+SUM(cash_book.pay_paypal) as total_pay

                              FROM (`cash_book`)
                              WHERE
                                  `cash_book`.`boat_trip_id`='".$trip_id."'
                                  and `cash_book`.`type`=1
                              GROUP BY `cash_book`.`boat_trip_id`
                                  ");
         $stmt -> execute();

         $row_array = $stmt->fetchAll();

         foreach($row_array as $row)
            {
            $value = $row['total_pay'];
            }

         return $value;
         }

function trip_cash_still_to_pay($trip_id,$dbh)
         {
         $stmt = $dbh->prepare("SELECT
                                       SUM(cash_book.value) as value,
                                       SUM(cash_book.pay_cash)+SUM(cash_book.pay_creditcard)+SUM(cash_book.pay_roomcharge)+SUM(cash_book.pay_bank)+SUM(cash_book.pay_paypal) as total_pay

                              FROM (`cash_book`)
                              WHERE
                                  `cash_book`.`boat_trip_id`='".$trip_id."'
                                  and `cash_book`.`type`=1
                              GROUP BY `cash_book`.`boat_trip_id`
                                  ");
         $stmt -> execute();

         $row_array = $stmt->fetchAll();

         foreach($row_array as $row)
            {
            $still_to_pay = $row['value']-$row['total_pay'];
            }

         return $still_to_pay;
         }

function invoice_still_to_pay($id_cash_book,$dbh)
         {
         $stmt = $dbh->prepare("SELECT
                                       SUM(cash_book.value) as value,
                                       SUM(cash_book.pay_cash)+SUM(cash_book.pay_creditcard)+SUM(cash_book.pay_roomcharge)+SUM(cash_book.pay_bank)+SUM(cash_book.pay_paypal) as total_pay

                              FROM (`cash_book`)
                              WHERE
                                  `cash_book`.`id_cash_book`='".$id_cash_book."'
                                  and `cash_book`.`type`=1
                              GROUP BY `cash_book`.`id_cash_book`
                                  ");
         $stmt -> execute();

         $row_array = $stmt->fetchAll();

         foreach($row_array as $row)
            {
            $still_to_pay = $row['value']-$row['total_pay'];
            }

         if($still_to_pay<>0)
            $still_to_pay = ' (still to pay)';
         else
            $still_to_pay = '';

         return $still_to_pay;
         }



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

      $invoice_still_to_pay = invoice_still_to_pay($bill_da[1],$dbh);

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


$stmt = $dbh->prepare("SELECT boat_trip.id_boat_trip,
                              boat_trip.date_from, boat_trip.date_to,
                              boat_trip.price_cabin_a, boat_trip.txt_cabin_a,
                              boat_trip.price_cabin_b, boat_trip.txt_cabin_b,
                              boat_trip.price_cabin_c, boat_trip.txt_cabin_c,
                              boat_trip.price_cabin_d, boat_trip.txt_cabin_d,
                              boat_trip.price_cabin_e, boat_trip.txt_cabin_e,

                              boat_trip.txt_route

                              FROM boat_trip
                              WHERE
                                  YEAR(`boat_trip`.`date_to`) = '".$period_year."'

                              ORDER BY boat_trip.date_to asc");
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

if($row['date_from']>$date_now)
   $calendar_bgcolor = ' style="background-color:#ccff99"';
elseif($row['date_from']<$date_now)
   $calendar_bgcolor = ' style="background-color:#F5F5F5"';

if($row['date_from']<$date_now and $row['date_to']>$date_now)
   $calendar_bgcolor = ' style="background-color:#73e600"';

    $still_to_pay_bgcolor = '';
    $still_to_pay = trip_cash_still_to_pay($row['id_boat_trip'],$dbh);
    if($still_to_pay<>0)
       $still_to_pay_bgcolor = ' style="background-color:#ffe0cc"';


   echo '<tr class=reports>
   <td align=center rowspan=8 '.$calendar_bgcolor.'>'.$row['date_from'].'<br>'.$row['date_to'].'<br>'.$row['txt_route'].'<br>days: '.$numberOfDays.'<br>nights: '.$numberOfNights.'
                    ';

	if($_SESSION['permission']=='rw')
		echo '<br><br><a class="pure-button pure-button-primary" href="empress_income.php?action=add&sel_trip='.$row['id_boat_trip'].'"><i class="fa fa-bed" aria-hidden="true"></i> Cabin Sell</a>
                      <br><br><a class="pure-button pure-button-primary" href="?action=edit&id='.$row['id_boat_trip'].'"><i class="far fa-edit" aria-hidden="true"></i> Trip Edit</a>
                      <br><br><a class="pure-button pure-button-primary" href="report_empress.php?action=trip_result&id_boat_trip='.$row['id_boat_trip'].'"><i class="far fa-chart-bar" aria-hidden="true"></i> Trip result</a>';

                echo '
                    </td>
   <td align=right>Price</td>

   <td class=cash width=200em>'.$formatter->formatCurrency($row['price_cabin_a'], 'USD').'</td>

   <td class=cash width=200em>'.$formatter->formatCurrency($row['price_cabin_b'], 'USD').'</td>

   <td class=cash width=200em>'.$formatter->formatCurrency($row['price_cabin_c'], 'USD').'</td>

   <td class=cash width=200em>'.$formatter->formatCurrency($row['price_cabin_d'], 'USD').'</td>

   <td class=cash width=200em>'.$formatter->formatCurrency($row['price_cabin_e'], 'USD').'</td>

   <td class=cash rowspan=8 '.$still_to_pay_bgcolor.'>';

                    echo $formatter->formatCurrency($still_to_pay, 'USD');

             echo '</td>
   </tr>';

   echo '<tr class="reports pure-table">
             <td align=right>Beds</td>
             <td align=center>'.show_beds($cab_beds['1'],$trip_beds_sell[$row['id_boat_trip']]['1']).'</td>
             <td align=center>'.show_beds($cab_beds['2'],$trip_beds_sell[$row['id_boat_trip']]['2']).'</td>
             <td align=center>'.show_beds($cab_beds['3'],$trip_beds_sell[$row['id_boat_trip']]['3']).'</td>
             <td align=center>'.show_beds($cab_beds['4'],$trip_beds_sell[$row['id_boat_trip']]['4']).'</td>
             <td align=center>'.show_beds($cab_beds['5'],$trip_beds_sell[$row['id_boat_trip']]['5']).'</td>';


      echo '
   </tr>';

   echo '<tr class="reports pure-table-odd">
             <td align=right>Sell</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_item_value[$row['id_boat_trip']]['1'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_item_value[$row['id_boat_trip']]['2'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_item_value[$row['id_boat_trip']]['3'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_item_value[$row['id_boat_trip']]['4'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_item_value[$row['id_boat_trip']]['5'], 'USD').'</td>';


      echo '
   </tr>';

/*
   echo '<tr class=reports>
             <td align=right>Paid</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_cabin_pay[$row['id_boat_trip']]['1'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_cabin_pay[$row['id_boat_trip']]['2'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_cabin_pay[$row['id_boat_trip']]['3'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_cabin_pay[$row['id_boat_trip']]['4'], 'USD').'</td>
             <td class=cash>'.$formatter->formatCurrency($trip_details_cabin_pay[$row['id_boat_trip']]['5'], 'USD').'</td>';


      echo '
   </tr>';
*/

   echo '<tr class="reports pure-table-odd">
             <td align=right>Invoice</td>
             <td>';
                 foreach($trip_details_bill_no[$row['id_boat_trip']]['1'] as $trip_det)
                     echo $trip_det.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_bill_no[$row['id_boat_trip']]['2'] as $trip_det)
                     echo $trip_det.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_bill_no[$row['id_boat_trip']]['3'] as $trip_det)
                     echo $trip_det.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_bill_no[$row['id_boat_trip']]['4'] as $trip_det)
                     echo $trip_det.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_bill_no[$row['id_boat_trip']]['5'] as $trip_det)
                     echo $trip_det.' ';
      echo '
             </td>';


      echo '
   </tr>';

   echo '<tr class=reports>
             <td align=right>BookingNo</td>
             <td>'.$trip_details_booking_no[$row['id_boat_trip']]['1'].'</td>
             <td>'.$trip_details_booking_no[$row['id_boat_trip']]['2'].'</td>
             <td>'.$trip_details_booking_no[$row['id_boat_trip']]['3'].'</td>
             <td>'.$trip_details_booking_no[$row['id_boat_trip']]['4'].'</td>
             <td>'.$trip_details_booking_no[$row['id_boat_trip']]['5'].'</td>';


      echo '
   </tr>';

   echo '<tr class="reports pure-table-odd">
             <td align=right>Booker</td>
             <td>'.$trip_details_booker_name[$row['id_boat_trip']]['1'].'</td>
             <td>'.$trip_details_booker_name[$row['id_boat_trip']]['2'].'</td>
             <td>'.$trip_details_booker_name[$row['id_boat_trip']]['3'].'</td>
             <td>'.$trip_details_booker_name[$row['id_boat_trip']]['4'].'</td>
             <td>'.$trip_details_booker_name[$row['id_boat_trip']]['5'].'</td>';


      echo '
   </tr>';


   echo '<tr class=reports>
             <td align=right>Name</td>
             <td>';
                 foreach($trip_details_client_name[$row['id_boat_trip']]['1'] as $client_name)
                     echo $client_name.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_client_name[$row['id_boat_trip']]['2'] as $client_name)
                     echo $client_name.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_client_name[$row['id_boat_trip']]['3'] as $client_name)
                     echo $client_name.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_client_name[$row['id_boat_trip']]['4'] as $client_name)
                     echo $client_name.' ';
      echo '
             </td>
             <td>';
                 foreach($trip_details_client_name[$row['id_boat_trip']]['5'] as $client_name)
                     echo $client_name.' ';
      echo '
             </td>';


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