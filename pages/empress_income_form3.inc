<?php

$stmt = $dbh->prepare("SELECT *
                              FROM personel
                              ORDER BY personel.name");
$stmt -> execute();
$personel_array = $stmt->fetchAll();

$stmt = $dbh->prepare("SELECT client.name, client.id_client
                              FROM client
                              ORDER BY client.name");
$stmt -> execute();
$client_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT booker.name as booker_name,booker.id_booker as booker_id, booker.commission, CONCAT(booker.id_booker,'|',booker.commission) as id_and_perc
                              FROM booker
                              ORDER BY booker.name");

$stmt -> execute();
$booker_array = $stmt->fetchAll();

$stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, boat_trip.id_boat_trip
                              FROM boat_trip
                              WHERE 1=1

                              ORDER BY boat_trip.date_to");

//YEAR(date_to)=".$period_year."

$stmt -> execute();
$boat_trip_array = $stmt->fetchAll();





$stmt = $dbh->prepare("SELECT currency.currency
                              FROM currency
                              ORDER BY currency.id_currency");
$stmt -> execute();
$currency_array = $stmt->fetchAll();


$id = $_GET['id'];
$date_now = date("Y-m-d");

if($action=='edit')
                   {
                   $stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, location.room_charge_perc, cash_book.location_id, cash_book.personel_id,
                                                 personel.name as pers_name, cash_book.type, cash_book.desc,
                              cash_book.id_cash_book, cash_book.date, cash_book.bill_no, cash_book.booking_no,

                              cash_book.booker_perc,
                              cash_book.booker_id,cash_book.booker_commission,
                              cash_book.agent_id,
                              cash_book.client_id,
                              cash_book.boat_trip_id,
                              cash_book.value,
                              cash_book.discount,
                              cash_book.value_before_discount,
                              cash_book.agent_commission,
                              cash_book.agent_perc,
                              cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
                              cash_book.pay_cash_deposit,
                              cash_book.pay_cash_deposit2,
                              cash_book.pay_cash_deposit3,
                              cash_book.pay_cash_deposit4,
                              cash_book.date_pay_deposit,
                              cash_book.date_pay_deposit2,
                              cash_book.date_pay_deposit3,
                              cash_book.date_pay_deposit4,
                              cash_book.pay_cash_part,
                              cash_book.pay_bank_part,
                              cash_book.pay_bank_deposit,
                              cash_book.pay_bank_deposit2,
                              cash_book.pay_bank_deposit3,
                              cash_book.pay_bank_deposit4,

                              cash_book.type_cost,


                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as sum_payments,

                              cash_book.lastchange

                              FROM (cash_book, cash_book_item, personel, location, cash_book_group)

                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              and cash_book_item.cash_book_id=cash_book.id_cash_book
                              and location.id_location=cash_book.location_id
                              and cash_book.id_cash_book='".$id."'
                              GROUP BY cash_book.id_cash_book
                              ");

                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $location_select = $edit_array[0][location_id];
                   $room_charge_select = $edit_array[0][room_charge_perc];
                   $desc = $edit_array[0][desc];
                   $bill_no = $edit_array[0][bill_no];
                   $date = $edit_array[0][date];
                   $price = $edit_array[0][price];
                   $currency = $edit_array[0][currency];
                   $value = $edit_array[0][value];
                   $discount = $edit_array[0][discount];
                   $value_before_discount = $edit_array[0][value_before_discount];
                   $booker_select = $edit_array[0][booker_id];
                   $booker_commission = $edit_array[0][booker_commission];
                   $booker_perc = $edit_array[0][booker_perc];
                   $booking_no = $edit_array[0][booking_no];
                   $agent_perc= $edit_array[0][agent_perc];
                   $agent_select = $edit_array[0][agent_id];
                   $agent_commission = $edit_array[0][agent_commission];
                   $boat_trip_select = $edit_array[0][boat_trip_id];

                   $type_cost = $edit_array[0][type_cost];

                   $client_select = $edit_array[0][client_id];

                   $pay_cash = $edit_array[0][pay_cash];
                   $pay_cash_part = $edit_array[0][pay_cash_part];
                   $pay_cash_deposit = $edit_array[0][pay_cash_deposit];
                   $pay_cash_deposit2 = $edit_array[0][pay_cash_deposit2];
                   $pay_cash_deposit3 = $edit_array[0][pay_cash_deposit3];
                   $pay_cash_deposit4 = $edit_array[0][pay_cash_deposit4];
                   $date_pay_deposit = $edit_array[0][date_pay_deposit];
                   $date_pay_deposit2 = $edit_array[0][date_pay_deposit2];
                   $date_pay_deposit3 = $edit_array[0][date_pay_deposit3];
                   $date_pay_deposit4 = $edit_array[0][date_pay_deposit4];
                   $pay_bank_part = $edit_array[0][pay_bank_part];
                   $pay_bank_deposit = $edit_array[0][pay_bank_deposit];
                   $pay_bank_deposit2 = $edit_array[0][pay_bank_deposit2];
                   $pay_bank_deposit3 = $edit_array[0][pay_bank_deposit3];
                   $pay_bank_deposit4 = $edit_array[0][pay_bank_deposit4];

                   $pay_creditcard = $edit_array[0][pay_creditcard];
                   $pay_roomcharge = $edit_array[0][pay_roomcharge];
                   $pay_bank = $edit_array[0][pay_bank];
                   $pay_paypal = $edit_array[0][pay_paypal];
                   $sum_payments = $edit_array[0][sum_payments];

                   $lastchange = $edit_array[0][lastchange];
                   $pers_name = $edit_array[0][pers_name];


                   $stmt = $dbh->prepare("SELECT activity.shortcut as act_shortcut, activity.name, activity.description, activity.id_activity,
                                                 cash_book_item.activity_id,cash_book_item.amount,cash_book_item.value, cash_book_item.id_cash_book_item

                              FROM cash_book_item, activity
                              WHERE cash_book_item.cash_book_id=".$id."
                              and cash_book_item.activity_id=activity.id_activity");

                   $stmt -> execute();
                   $cash_book_item_array = $stmt->fetchAll();

                   }

if($action=='add')
                   {
                   $date = date('Y-m-d');
                   $location_select = $_SESSION[personel_location];
                   $boat_trip_select = $_GET['sel_trip'];
                   $client_select = $_GET['sel_client'];
                   }


$ontripsell = $_GET['ontripsell'];

if($type_cost == 2)  $ontripsell=1;

if($ontripsell==1)
    $activity_limit = '71,86,87,88,89,90,91,92';
else
    $activity_limit = '73,74,75,76,77,78,79,80,81,82,83,84,85';

$stmt = $dbh->prepare("SELECT activity.id_activity, activity.shortcut as act_shortcut, activity.name as act_name
                              FROM activity
                              WHERE id_activity IN (".$activity_limit.")
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();


?>



<div>


<?php if($ontripsell==1) echo '<h2>Empress - "On trip" Sell</h2>'; else echo '<h2>Empress - Cabin Sell</h2>'; ?>


<form class="pure-form pure-form-aligned" action="?action=post" method="POST" name="income" onsubmit="return validateForm()">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >
<input type="hidden" name="type" value="1" >
<input type="hidden" name="location_id" value="14" >
<input type="hidden" name="cash_book_group_id" value="14" >


<?php

if($ontripsell==1)
   echo '<input type="hidden" name="type_cost" value="2" >';

?>


            <div class="pure-control-group">

                <?php if($ontripsell=='') {  ?>
                <label for="client_id">Guest Name</label>
                <select name="client_id" id="client_id" class="pure-input-1-4" style='width:12em'>
                <?php
                     foreach($client_array as $row)
                        {
                        if( $client_select == $row['id_client'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_client'].'" '.$sel.'>'.$row['name'].'</option>';
                        }
                ?>
                </select>
                <a href="client.php?action=add&back=emp_add&sel_trip=<?php echo $boat_trip_select; ?>" id="client_link"><i class="fas fa-plus-circle"></i></a>
                <?php };?>

                <label for="boat_trip_id">Trip</label>
                <select name="boat_trip_id" id="boat_trip_id" class="pure-input-1-1">
                <?php
                     foreach($boat_trip_array as $row)
                        {
                        //koloruje aktualny trip
                        $option_bgcolor = '';
                        if($row['date_from']<$date_now)
                           $option_bgcolor = ' style="background-color:#e6e6e6"';
                        if($row['date_from']<$date_now and $row['date_to']>$date_now)
                           $option_bgcolor = ' style="background-color:#73e600"';

                        if( $boat_trip_select == $row['id_boat_trip']) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_boat_trip'].'" '.$sel.$option_bgcolor.'>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</option>';
                        }
                ?>
                </select>
            </div>


            <div class="pure-control-group">
              <?php if($ontripsell=='') {  ?>
                <label for="name">Invoice Date</label>
              <?php } else { ?>
                <label for="name">Date</label>
              <?php };?>

                <input type="date" name="date" id="date" value="<?php echo $date; ?>" class="pure-input-1-1"  style='width:12em'>

<!--                <input type="text" name="desc" id="description" value="<?php echo $desc; ?>" class="pure-input-1-5" autofocus> -->

              <?php if($ontripsell=='') {  ?>
                <label for="bill_no">Invoice No.</label>
              <?php } else { ?>
                <label for="bill_no">Bill No.</label>
              <?php };?>


                <input type="text" name="bill_no" id="bill_no" value="<?php echo $bill_no; ?>" class="pure-input-1-1" style='width:8em;'>

<!--
                <label for="currency">Document currency</label>
                <select name="currency" id="currency" class="pure-input-1-4" style='width:5em'>
                <?php
                     foreach($currency_array as $row)
                        {
                        if( $location_select == $row['currency'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['currency'].'" '.$sel.'>'.$row['currency'].'</option>';
                        }
                ?>
                </select>


            </div>
-->

            <div class="pure-control-group">

            <table id="dataTable" class="form" border="0">
 <tbody>
<?php
if($action<>'edit')
    {   ?>

  <tr>
	<td >
		<input type="checkbox" name="chkbox[]"/>
	</td>
	<td>
                <label for="activity_id">Activity</label>
                <select name="activity_id[]" id="activity_id" class="pure-input" style='width:13em'>
                <?php
                     foreach($activity_array as $row)
                        {
                        if( $activity_select == $row['id_activity'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_activity'].'" '.$sel.'>'.$row['act_shortcut'].': '.$row['act_name'].'</option>';
                        }
                ?>
                </select>
	</td>
	<td>
<!--
                <label for="name">Pax.</label>
                <input type="text" name="pcs_no[]" id="bill_no" value="<?php echo $pcs_no; ?>" class="pure-input-1-1" style='width:5em; text-align:center'>
-->	</td>
	<td>

                <label for="name">Activity price</label>
                <input type="text" name="price[]" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$price)); ?>" class="money2" style='width:8em; text-align:right' onkeyup="findSumAct()">
        </td>
        <td>
                 <select name="currency[]" id="currency" class="pure-input" style='width:4.5em'>

                 <?php
                 echo '<option value="'.$row['currency'].'">IDR</option>';
                 ?>

                 </select>
        </td>
  </tr>

<?php
     }
elseif($action=='edit')
    {

   ?>




 <?php

                     foreach($cash_book_item_array as $row)
                        {
                        $activity_select = $row[id_activity];
 ?>
  <tr>
	<td >
	</td>
	<td>
                <input type="hidden" name="id_cash_book_item[]" value="<?php echo $row[id_cash_book_item] ?>" >

                <label for="activity_id">Activity</label>

                <select name="activity_id[]" id="activity_id" class="pure-input" style='width:20em'>
                <?php
                     foreach($activity_array as $row2)
                        {
                        if( $activity_select == $row2['id_activity'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row2['id_activity'].'" '.$sel.'>'.$row2['act_shortcut'].': '.$row2['act_name'].'</option>';
                        }
                ?>
                </select>
	</td>
	<td>
<!--                <label for="name">Pax.</label>
                <input type="text" name="pcs_no[]" id="bill_no" value="<?php echo $row[amount]; ?>" class="pure-input-1-1" style='width:5em; text-align:center'>
-->
	</td>
	<td>

                <label for="name">Activity price</label>
                <input type="text" name="price[]" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$row[value])); ?>" class="money2" style='width:8em; text-align:right' onkeyup="findSumAct()">
       </td>
       <td>
                 <select name="currency[]" id="currency" class="pure-input" style='width:4.5em'>

                 <?php
                 echo '<option value="'.$row['currency'].'">IDR</option>';
                 ?>

                 </select>
        </td>
  </tr>

 <?php
                       }
 ?>

<?php

    }

?>
 </tbody>
</table>
</div>


<?php
if($action<>'edit')
    {   ?>


            <div class="pure-controls">
<style scoped>
 .button-xsmall {
                font-size: 70%;
            }
</style>

  <input type="button" class="button-xsmall pure-button" value="+ activity" onClick="addRow('dataTable')" accesskey="a" />
  <input type="button" class="button-xsmall pure-button" value="remove checked" onClick="deleteRow('dataTable')" accesskey="r" />
                                     <br><br>
            </div>
<?php

    }

?>

            <div class="pure-control-group">
              <?php

                  if($ontripsell=='')
                     {
                     echo '<label for="name">Activity before comm.</label>';
                     $val_inp_type = 'text';
                     }
                  else
                     {
                     $val_inp_type = 'hidden';
                     };

               echo '<input type="'.$val_inp_type.'" name="value_before_discount" id="value_before_discount" value="'.str_replace('.00','',str_replace(',','',$value_before_discount)).'" class="money2" style="width:8em; text-align:right" / readonly>';

                     ?>
            </div>


          <?php if($ontripsell=='') {  ?>
            <div class="pure-control-group">
                <label for="booker_id">Booker</label>
                <select name="booker_id" id="booker_id" class="pure-input-1-6" style='width:8em;' onkeyup="findSumBooker()">
                        <option value="0|0">no booker</option>
                <?php
                     foreach($booker_array as $row)
                        {
                        if( $booker_select == $row['booker_id'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_and_perc'].'" '.$sel.'>'.$row['booker_name'].'</option>';
                        }
                ?>
                </select>
                <label for="name">%</label>
                <input type="text" name="booker_perc" id="booker_perc" value="<?php echo $booker_perc; ?>" class="pure-input-1-1" style='width:5em; text-align:center' onkeyup="findSumBooker()">
                <label for="booking_no">Booking No:</label>
                <input type="text" name="booking_no" id="booking_no" value="<?php echo $booking_no; ?>" style='width:9em' onkeyup="findSumBooker()">
                <label for="name">Commission</label>
                <input type="text" name="booker_commission" id="booker_commission" value="<?php echo str_replace('.00','',str_replace(',','',$booker_commission)); ?>" class="money2" style='width:8em; text-align:right' readonly>
            </div>
           <?php }; ?>

            <div class="pure-control-group">

                <label for="name">TOTAL Amount</label>
                <input type="text" name="total_act" id="total_act" value="<?php echo str_replace('.00','',str_replace(',','',$value));?>" class="money2" style='width:8em; text-align:right' / readonly>
            </div>


<hr>
            <div class="pure-control-group">
                <p align=center>Payment:</p>
            </div>

            <table id="dataTableP" class="form" border="0">
 <tbody>
<?php
if($action<>'edit')
    {   ?>

  <tr>
	<td >
		<input type="checkbox" name="chkbox[]"/>
	</td>
	<td>

            <div class="pure-control-group">
                <select name="payment_type[]" id="payment_type" class="pure-input" style='width:8em'>
                 echo '<option value="cash">Cash</option>';
                 echo '<option value="cash_dep">Cash Deposit</option>';
                 echo '<option value="bank">Bank</option>';
                 echo '<option value="bank_dep">Bank Deposit</option>';
                 echo '<option value="roomcharge">Roomcharge</option>';
                 echo '<option value="creditcard">Creditcard</option>';
                 echo '<option value="paypal">Paypal</option>';
                 </select>
                <input type="text" name="payment_value" id="payment_value" value="<?php echo str_replace('.00','',str_replace(',','',$pay_cash_part)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
                <select name="currency[]" id="currency" class="pure-input" style='width:4.5em'>

                 <?php
                 echo '<option value="'.$row['currency'].'">IDR</option>';
                 ?>

                 </select>
                <label for="name">Payment Date</label>
                <input type="date" name="date_pay_deposit" id="date_pay_deposit" value="<?php echo $date_pay_deposit; ?>" class="pure-input-1-1"  style='width:10em'>
            </div>
        </td>
  </tr>

 <?php
                       }
 ?>

<?php



?>
 </tbody>
</table>
</div>


<?php
if($action<>'edit')
    {   ?>


            <div class="pure-controls">
<style scoped>
 .button-xsmall {
                font-size: 70%;
            }
</style>

  <input type="button" class="button-xsmall pure-button" value="+ payment type" onClick="addRow('dataTableP')"  />
  <input type="button" class="button-xsmall pure-button" value="remove checked" onClick="deleteRow('dataTableP')"  />
                                     <br><br>
            </div>
<?php

    }

?>













            <div class="pure-control-group">
                <label for="total_payment">Total payment</label>
                <input type="text" name="total_payment" id="total_payment" value="<?php echo str_replace('.00','',str_replace(',','',$sum_payments)); ?>" class="money" style='width:8em; text-align:right' readonly>

                <label for="still_to_pay">Still to pay</label>
                <input type="text" name="still_to_pay" id="still_to_pay" value="<?php echo str_replace('.00','',str_replace(',','',$value-$sum_payments)); ?>" class="money" style='width:8em; text-align:right' readonly>

            </div>

            <div class="pure-controls" align=center>



                <?php

                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary" >Submit</button>';

                      if($action<>'add' and $_SESSION['location_id_limit']==0)
                      echo ' <a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }


                if($action=='edit')
                   {
                   echo '<br><br><font color=grey>created by: '.$pers_name.', lastchange date: '.$lastchange.'</font>';
                   }


                 ?>




            </div>
        </fieldset>
    </form>


<?php
if($action=='edit' and $room_charge_select<>0)
                   {
                   $scuba_perc = 100-$room_charge_select;
                   $hotel_amount = $value*$room_charge_select/100;
                   $scuba_amount = $value-$hotel_amount;
                   $total_amount = $hotel_amount+$scuba_amount;

                   echo '<div class="pure" align=center>';
                   echo '<table class="pure-table pure-table-horizontal pure-table-striped">';
                   echo '<tr><td colspan=3>Room charge</td></tr>';
                   echo '<tr><td>Hotel</td><td align=right>'.$room_charge_select.' %</td><td align=right>'.$hotel_amount.'</td></tr>';
                   echo '<tr><td>Scuba</td><td align=right>'.$scuba_perc.' %</td><td align=right>'.$scuba_amount.'</td></tr>';
                   echo '<tr><td>Total</td><td align=right></td><td align=right>'.$total_amount.'</td></tr>';
                   echo '</table></div>';
                   }
?>

</div>






<script type="text/javascript">

function findGetParameter(parameterName) {
    var result = null,
        tmp = [];
    location.search
        .substr(1)
        .split("&")
        .forEach(function (item) {
          tmp = item.split("=");
          if (tmp[0] === parameterName) result = decodeURIComponent(tmp[1]);
        });
    return result;
}


function findSumAct(){
    var arr = document.getElementsByName('price[]');
    var tot=0;
    var booker_perc=0;
    var ontripsell = findGetParameter(ontripsell);

    if(ontripsell != 1)
      {
      var booker_perc_tab = document.getElementById('booker_perc');

      if(booker_perc_tab)
        var booker_perc=parseInt(document.getElementById('booker_perc').value.split(",").join("")) || 0;
      }

    var total_act=parseInt(document.getElementById('total_act').value.split(",").join("")) || 0;

    for(var i=0;i<arr.length;i++){
        if(parseInt(arr[i].value.split(",").join("")))
            tot += parseInt(arr[i].value.split(",").join(""));
    }
    // bylo ale nie uzywamy discount document.getElementById('value_before_discount').value = tot;
    // bylo ale nie uzywamy discount document.getElementById('total_act').value = tot-tot*document.getElementById('discount').value/100;


    document.getElementById('value_before_discount').value = tot;

    tot = tot - tot * booker_perc/100;

    document.getElementById('total_act').value = tot;

if(ontripsell != 1)
  {
  findSumBooker();
  }

findStillToPay();
}


function findSumPayment(){
    var tot=0;
    var cash_part=parseInt(document.getElementById('cash_part').value.split(",").join("")) || 0;
    var pay_cash_deposit=parseInt(document.getElementById('pay_cash_deposit').value.split(",").join("")) || 0;
    var pay_cash_deposit2=parseInt(document.getElementById('pay_cash_deposit2').value.split(",").join("")) || 0;
    var pay_cash_deposit3=parseInt(document.getElementById('pay_cash_deposit3').value.split(",").join("")) || 0;
    var pay_cash_deposit4=parseInt(document.getElementById('pay_cash_deposit4').value.split(",").join("")) || 0;
    var pay_bank_deposit=parseInt(document.getElementById('pay_bank_deposit').value.split(",").join("")) || 0;
    var pay_bank_deposit2=parseInt(document.getElementById('pay_bank_deposit2').value.split(",").join("")) || 0;
    var pay_bank_deposit3=parseInt(document.getElementById('pay_bank_deposit3').value.split(",").join("")) || 0;
    var pay_bank_deposit4=parseInt(document.getElementById('pay_bank_deposit4').value.split(",").join("")) || 0;
    var creditcard=parseInt(document.getElementById('creditcard').value.split(",").join("")) || 0;
    var pay_bank_part=parseInt(document.getElementById('pay_bank_part').value.split(",").join("")) || 0;
    var paypal=parseInt(document.getElementById('paypal').value.split(",").join("")) || 0;
    var roomcharge=parseInt(document.getElementById('roomcharge').value.split(",").join("")) || 0;

    tot = tot + cash_part;
    tot = tot + pay_cash_deposit;
    tot = tot + pay_cash_deposit2;
    tot = tot + pay_cash_deposit3;
    tot = tot + pay_cash_deposit4;
    tot = tot + pay_bank_deposit;
    tot = tot + pay_bank_deposit2;
    tot = tot + pay_bank_deposit3;
    tot = tot + pay_bank_deposit4;
    tot = tot +  creditcard;
    tot = tot +  pay_bank_part;
    tot = tot +  paypal;
    tot = tot +  roomcharge;

    document.getElementById('total_payment').value = tot;

    findStillToPay();
}



function findSumBooker(){
             var tot=0;
            var arr = document.getElementsByName('price[]');

            for(var i=0;i<arr.length;i++){
                    if(parseInt(arr[i].value.split(",").join("")))
                    tot += parseInt(arr[i].value.split(",").join(""));
               }


                var booker_perc=parseInt(document.getElementById('booker_perc').value.split(",").join("")) || 0;
//    var total_act=parseInt(document.getElementById('total_act').value.split(",").join("")) || 0;

              tot = tot * booker_perc/100;


              document.getElementById('booker_commission').value = tot;
    findSumAct();
}

function findStillToPay(){
    var tot=0;
    var total_payment=parseInt(document.getElementById('total_payment').value.split(",").join("")) || 0;
    var total_act=parseInt(document.getElementById('total_act').value.split(",").join("")) || 0;

    tot = total_act - total_payment;


    document.getElementById('still_to_pay').value = tot;
}

<?php
/*
function validateForm(){
    if(document.getElementById("total_payment").value.split(",").join("") == document.getElementById("total_act").value.split(",").join("") ) {
        document.income.submit();
    } else {
        alert("TOTAL after discount must be same as Total payment");
        document.getElementById('total_payment').style.backgroundColor = "#ffcccc";
        document.getElementById('total_act').style.backgroundColor = "#ffcccc";
        return false;
    }
 }
*/
?>

function validateForm(){
    if(1==1) {
        document.income.submit();
        }
   }

$(document).ready(function() {

    $("#booker_id option").filter(function() {
        return $(this).val() == $("#booker_perc").val();
    }).attr('selected', true);


    $("#booker_id").on("change", function() {

        $("#booker_perc").val($(this).find("option:selected").attr("value").toString().split('|')[1]);

    });
});


var sel = document.getElementById('boat_trip_id');
sel.onchange = function () {
    document.getElementById("client_link").href = "client.php?action=add&back=emp_add&sel_trip=" + this.value;
}

</script>
