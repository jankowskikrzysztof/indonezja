<script>
function changePeriod(url)
{
    location.href = 'empress_income.php?period=' + url;
}

function changeTrip(url)
{
    location.href = 'empress_income.php?trip_id=' + url;
}
</script>


<div align=center class="pure-form">

<?php
if($_GET['trip_id']<>0)
    echo "<input type=text value='trip specified' disabled>";
else
    echo "<input type=month value='".$period."' onchange='changePeriod(this.value)'>";


$stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, id_boat_trip as boat_trip_id
                              FROM boat_trip
                              ORDER BY date_to");
$stmt -> execute();
$boat_trip_array = $stmt->fetchAll();
$date_now = date("Y-m-d");
?>


                <select name="trip_id" id="boat_trip_id" class="pure-input-1-1" onchange="changeTrip(this.value)">
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
<?php
if($_SESSION['permission']=='rw')
echo '  
      <th colspan=8>
          <a class="pure-button pure-button-primary" href="?period='.$period.'&action=add"><i class="fa fa-bed" aria-hidden="true"></i> Cabin Sell</a>
          <a class="pure-button pure-button-primary" href="?period='.$period.'&action=add&ontripsell=1" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd "on trip" Sell</a>
      </th>
      <th colspan=3 align=center>
<a href=?period='.$period.'&trip_id='.$_GET['trip_id'].'&only_cabin=1>Only Cabin Sell</a>    
      </th>';
?>
  </tr>
  <tr align=center>
     <th>Day</th>
     <th>Trip</th>
     <th>Client<br>/Booker</th>
     <th>Cabin/Bed</th>
     <th>Invoice No.</th>
     <th>Amount</th>
     <th>Payment<br>type</th>
     <th>Payment</th>
     <th>Still to pay</th>
     <th>PDF</th>
     <th>Edit</th>
  </tr>
 </thead>

<?php

setlocale(LC_MONETARY,'id_ID');


if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';


$sel_trip = '';
if($_GET['trip_id'] <>0)
   $sel_trip = 'and cash_book.boat_trip_id='.$_GET['trip_id'];

$sel_only_cabin = '';
if($_GET['only_cabin'] ==1)
   $sel_only_cabin = 'and cash_book_item.activity_id IN (73,74,75,76,77,78,79,80,81,82,83,84,85)';

$sel_year_month='';
if($sel_trip == '')
   $sel_year_month="AND MONTH(`cash_book`.`date`) = '".$period_month."' AND YEAR(`cash_book`.`date`) = '".$period_year."'";



$sql_select = "SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
                              activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no, 
                              location_groups.name as loc_grp_name, DAY(cash_book.date) as dzien,

                              cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
                              cash_book.pay_cash_currency,cash_book.pay_creditcard_currency,cash_book.pay_roomcharge_currency,cash_book.pay_bank_currency,cash_book.pay_paypal_currency,
                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
                              cash_book.pay_cash_currency+cash_book.pay_creditcard_currency+cash_book.pay_roomcharge_currency+cash_book.pay_bank_currency+cash_book.pay_paypal_currency as total_pay_currency,
                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.value,
                              cash_book.value_currency,

                              cash_book.currency,
                              cash_book.currency_second,

                              GROUP_CONCAT(activity.shortcut) as shortcut,
                              booker.name as booker_name,
                              client.name as client_name,
                              cash_book.client_id,
                              GROUP_CONCAT(settlements.id_settlements) as settlements, 

                              (SELECT SUM(value) FROM settlements WHERE settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book') as set_value

                              FROM (cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join client on client.id_client=cash_book.client_id

                              left join settlements on settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book'

                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              ".$sel_trip."
                              ".$sel_year_month."
                              ".$period_limit."
                              ".$sel_only_cabin."

                              and location.id_location=14

                              and activity.id_activity=cash_book_item.activity_id
                              and cash_book_item.cash_book_id=cash_book.id_cash_book
                              and location.id_location=cash_book.location_id
                              and location.location_groups_id=location_groups.id_location_groups
                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date desc, cash_book.bill_no DESC";

//echo '<pre>'.$sql_select.'</pre>';

$stmt = $dbh->prepare($sql_select);

$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $route_txt = '';
   if($row['boat_trip_id']>0)
      $route_txt = '<font style="font-size:80%;">'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'</font>';

   //$still_to_pay = $row['value']-$row['total_pay'];
   $still_to_pay = $row['value_currency']-$row['set_value'];


   $payment_type = '';
   if($row['pay_cash']<>0)
      {
      $still_to_pay = $still_to_pay - $row['pay_cash_currency'];
      $payment_type .= 'cash,';
      }
   if($row['pay_creditcard']<>0)
      {
      $still_to_pay = $still_to_pay - $row['pay_creditcard_currency'];
      $payment_type .= 'creditcard,';
      }
   if($row['pay_roomcharge']<>0)
      $payment_type .= 'roomcharge,';
   if($row['pay_bank']<>0 or $row['set_value']<>0)
      $payment_type .= 'bank,';
   if($row['pay_paypal']<>0)
      $payment_type .= 'paypal,';

   if($row['value']==0)
      $value_error_color = ' style="background-color:#FFCCCB"';
   else
      $value_error_color = '';


   if($row['currency_second'] <> '' and $row['currency_second'] <> '0.0' and $row['currency']<>$row['currency_second'])
      {
      $value_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['value_currency'], $row['currency_second']).' '.$row['currency_second'].'</font>';
      //$still_to_pay_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['value_currency']-$row['total_pay_currency'], $row['currency_second']).' '.$row['currency_second'];
      $total_pay_currency = '<br><font style="font-size:80%;">'.$formatter->formatCurrency($row['total_pay_currency']+$row['set_value'], $row['currency_second']).' '.$row['currency_second'];
      }
    else
      {$value_currency=''; $still_to_pay_currency = ''; $total_pay_currency = '';}


   $client_name = '';
   if($row['client_id']<>0)
      $client_name = '<font style="font-size:80%;"><a href="client.php?action=edit&id='.$row['client_id'].'">'.$row['client_name'].'</a></font>';

   echo '<tr>
   <td align=center>'.$row['dzien'].'</td>
   <td align=center>'.$route_txt.'</td>
   <td>'.$client_name.'<br><font style="font-size:80%;">'.$row['booker_name'].'</font></td>
   <td>'.$row['shortcut'].'</td>
   <td>'.$row['bill_no'].'</td>
   <td align=right'.$value_error_color.'>'.$formatter->formatCurrency($row['value'], 'IDR').' '.$row['currency'].$value_currency.'</td>
   <td align=center>'.$payment_type.'</td>
   <td align=right>'.$formatter->formatCurrency($row['total_pay'], 'IDR').' '.$row['currency'].$total_pay_currency.'</td>
   <td align=right>'.$formatter->formatCurrency($still_to_pay, 'IDR').'</td>
   <td align=right>';
   // '.$row['currency'].$still_to_pay_currency.'
   //($_SESSION['login'] == 'kjankowski' or $_SESSION['login'] == 'przemek') and 
   if($row['boat_trip_id']<>0 and $row['client_id']<>0)
        {
        echo '<a href="invoice_pdf.php?action=xlse&id='.$row['id_cash_book'].'"><i class="far fa-file-pdf fa-2x" aria-hidden="true"></i></a> ';
        }

   echo '</td><td>';


	if($_SESSION['permission']=='rw' and $row['boat_trip_id']<>0)
		echo '<a href="?action=edit&period='.$period.'&id='.$row['id_cash_book'].'"><i class="far fa-edit fa-2x" aria-hidden="true"></i></a>';
	if($_SESSION['permission']=='rw' and $row['boat_trip_id']==0)
		echo '<a href="income.php?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit fa-2x" aria-hidden="true"></i></a>';

        


   echo '</td>
   </tr>';

   $sum_value += $row['value'];
   $sum_total_pay += $row['total_pay'];
   $sum_still_to_pay += $still_to_pay;

   }


?>
<tr>
  <td colspan=5></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
  <td></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_total_pay, 'IDR'); ?></td>
  <td class=cash><?php echo $formatter->formatCurrency($sum_still_to_pay, 'IDR'); ?></td>
  <td></td>
  <td></td>
</tr>

</table>
