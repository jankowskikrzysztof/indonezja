<?php

$stmt = $dbh->prepare("SELECT *
                              FROM personel
                              ORDER BY personel.name");
$stmt -> execute();
$personel_array = $stmt->fetchAll();


if($personel_location_selected <> '' and $action<>'edit')
$def_location = 'and location.id_location='.$personel_location_selected;
else
$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

if($action<>'edit')
  $without_empress = 'and location.id_location<>14';
else
  $without_empress = '';

$stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location, location.room_charge_perc
                              FROM location
                              WHERE 1=1
                              ".$def_location."
                              ".$location_limit."
                              ".$without_empress."
                              ORDER BY location.name");

$stmt -> execute();
$location_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT instructor.name as instr_name,instructor.id_instructor, instructor.percentage, CONCAT(instructor.id_instructor,'|',instructor.percentage) as id_and_perc
                              FROM instructor
                              ORDER BY instructor.name");

$stmt -> execute();
$instructor_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT activity.id_activity, activity.shortcut as act_shortcut, activity.name as act_name
                              FROM activity
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();

$stmt = $dbh->prepare("SELECT currency.currency
                              FROM currency
                              ORDER BY currency.id_currency");
$stmt -> execute();
$currency_array = $stmt->fetchAll();


$id = $_GET['id'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, location.room_charge_perc, cash_book.location_id, cash_book.personel_id,
                                                 personel.name as pers_name, cash_book.type, cash_book.desc,
                              cash_book.id_cash_book, cash_book.date, cash_book.bill_no,

                              cash_book.booker_perc,
                              cash_book.booker_id,cash_book.booker_commission,
                              cash_book.instructor_id,
                              cash_book.value,
                              cash_book.discount,
                              cash_book.value_before_discount,
                              cash_book.instructor_commission,
                              cash_book.instructor_perc,
                              cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
                              cash_book.pay_cash_deposit, cash_book.pay_cash_part,
                              cash_book.pay_bank_deposit, cash_book.pay_bank_part,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as sum_payments,

                              cash_book.lastchange

                              FROM cash_book, cash_book_item, personel, location, cash_book_group
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
                   $booker_id = $edit_array[0][booker_id];
                   $booker_commission = $edit_array[0][booker_commission];
                   $booker_perc = $edit_array[0][booker_perc];
                   $instructor_perc= $edit_array[0][instructor_perc];
                   $instructor_select = $edit_array[0][instructor_id];
                   $instructor_commission = $edit_array[0][instructor_commission];
                   $pay_cash = $edit_array[0][pay_cash];
                   $pay_cash_part = $edit_array[0][pay_cash_part];
                   $pay_cash_deposit = $edit_array[0][pay_cash_deposit];
                   $pay_creditcard = $edit_array[0][pay_creditcard];
                   $pay_roomcharge = $edit_array[0][pay_roomcharge];
                   $pay_bank = $edit_array[0][pay_bank];
                   $pay_bank_part = $edit_array[0][pay_bank_part];
                   $pay_bank_deposit = $edit_array[0][pay_bank_deposit];
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
                   }



?>



<div>



<form class="pure-form pure-form-aligned" action="?action=post" method="POST" name="income" onsubmit="return validateForm()">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >
<input type="hidden" name="type" value="1" >
<input type="hidden" name="cash_book_group_id" value="14" >


            <div class="pure-control-group">
                <label for="name">Date</label>
                <input type="date" name="date" id="date" value="<?php echo $date; ?>" class="pure-input-1-1">

                <label for="location_id">Location</label>
                <select name="location_id" id="location_id" class="pure-input-1-1">
                <?php
                     foreach($location_array as $row)
                        {
                        if( $location_select == $row['id_location'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_location'].'" '.$sel.'>'.$row['loc_name'].'</option>';
                        }
                ?>
                </select>
            </div>


            <div class="pure-control-group">
                <label for="desc">Guest Name</label>
                <input type="text" name="desc" id="description" value="<?php echo $desc; ?>" class="pure-input-1-5" autofocus>

                <label for="bill_no">Bill No.</label>
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

-->

            </div>

            <div class="pure-controls">

            <table class="pure-form" border="0"  id="dataTable">
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
                <select name="activity_id[]" id="activity_id" class="pure-input" style='width:11em'>
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
                <label for="name">Pax.</label>
                <input type="text" name="pcs_no[]" id="bill_no" value="<?php echo $pcs_no; ?>" class="pure-input-1" style='width:3em; text-align:center'>
	</td>
	<td>

                <label for="name">Activity price</label>
                <input type="text" name="price[]" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$price)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumAct()">

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

                <select name="activity_id[]" id="activity_id" class="pure-input" style='width:11em'>
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
                <label for="name">Pax.</label>
                <input type="text" name="pcs_no[]" id="bill_no" value="<?php echo $row[amount]; ?>" class="pure-input-1" style='width:3em; text-align:center'>
	</td>
	<td>

                <label for="name">Bill Value</label>
                <input type="text" name="price[]" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$row[value])); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumAct()">

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
                <!--
                <label for="name">Activity TOTAL</label>
                <input type="text" name="value_before_discount" id="value_before_discount" value="<?php echo str_replace('.00','',str_replace(',','',$value_before_discount));?>" class="money" style='width:8em; text-align:right' / readonly>
                <label for="name">Discount %</label>
                <input type="text" name="discount" id="discount" value="<?php echo $discount;?>" class="pure-input-1-1" style='width:5em; text-align:center'  onkeyup="findSumAct()">
                -->

                <label for="name">TOTAL Amount</label>
                <input type="text" name="total_act" id="total_act" value="<?php echo str_replace('.00','',str_replace(',','',$value));?>" class="money" style='width:8em; text-align:right' / readonly>
            </div>

            <div class="pure-control-group">
                <label for="booker_id">Booker</label>
                <input type="text" name="booker_id" id="booker_id" value="<?php echo $booker_id; ?>" class="pure-input-1-1"  style='width:8em;'>
                <label for="name">%</label>
                <input type="text" name="booker_perc" id="booker_perc" value="<?php echo $booker_perc; ?>" class="pure-input-1-1" style='width:5em; text-align:center' onkeyup="findSumBooker()">
                <label for="name">Commission</label>
                <input type="text" name="booker_commission" id="booker_commission" value="<?php echo str_replace('.00','',str_replace(',','',$booker_commission)); ?>" class="money" style='width:8em; text-align:right' readonly>
            </div>
<!--
            <div class="pure-control-group">
                <label for="name">Instructor</label>
                <select name="instructor_id" id="instructor_id" class="pure-input-1-6" style='width:8em;'>
                        <option value="0|0">no instructor</option>
                <?php
                     foreach($instructor_array as $row)
                        {
                        if( $instructor_select == $row['id_instructor'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_and_perc'].'" '.$sel.'>'.$row['instr_name'].'</option>';
                        }
                ?>
                </select>
                <label for="name">%</label>
                <input type="text" name="instructor_perc" id="instructor_perc" value="<?php echo $instructor_perc; ?>" class="pure-input-1-1" style='width:5em; text-align:center' onkeyup="findSumInstructor()">
                <label for="name">Commission</label>
                <input type="text" name="instructor_commission" id="instructor_commission" value="<?php echo str_replace('.00','',str_replace(',','',$instructor_commission)); ?>" class="money" style='width:8em; text-align:right' readonly>
            </div>
-->
<hr>
            <div class="pure-control-group">
                <p align=center>Payment for activities:</p>
            </div>

            <div class="pure-control-group">
                <label for="cash_part">Cash</label>
                <input type="text" name="cash_part" id="cash_part" value="<?php echo str_replace('.00','',str_replace(',','',$pay_cash_part)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
                <label for="cash_deposit">Cash Deposit</label>
                <input type="text" name="cash_deposit" id="cash_deposit" value="<?php echo str_replace('.00','',str_replace(',','',$pay_cash_deposit)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
            </div>

            <div class="pure-control-group">
                <label for="pay_bank_part">Bank Transfer</label>
                <input type="text" name="pay_bank_part" id="pay_bank_part" value="<?php echo str_replace('.00','',str_replace(',','',$pay_bank_part)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
                <label for="pay_bank_deposit">Bank Deposit</label>
                <input type="text" name="pay_bank_deposit" id="pay_bank_deposit" value="<?php echo str_replace('.00','',str_replace(',','',$pay_bank_deposit)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
            </div>

            <div class="pure-control-group">
                <label for="roomcharge">Roomcharge</label>
                <input type="text" name="roomcharge" id="roomcharge" value="<?php echo str_replace('.00','',str_replace(',','',$pay_roomcharge)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
            </div>
            <div class="pure-control-group">
                <label for="creditcard">Creditcard</label>
                <input type="text" name="creditcard" id="creditcard" value="<?php echo str_replace('.00','',str_replace(',','',$pay_creditcard)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
            </div>
            <div class="pure-control-group">
                <label for="paypal">Paypal</label>
                <input type="text" name="paypal" id="paypal" value="<?php echo str_replace('.00','',str_replace(',','',$pay_paypal)); ?>" class="money" style='width:8em; text-align:right' onkeyup="findSumPayment()" >
            </div>

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
                    $stmt = $dbh->prepare("SELECT personel.login
                    FROM logs, personel
                    WHERE personel.id_personel=logs.personel_id
                    and logs.target_table='cash_book'
                    and logs.target_id=".$id."
                    ORDER BY insertdate DESC
                    LIMIT 1");
                    $stmt -> execute();
                    $last_change = $stmt->fetchAll();  

                    foreach($last_change as $row)
                            {
                            $pers_last_change = $row['login'];
                            }

                    if($_SESSION['user_id'] == 1) $logs_link = '<a href="logs.php?action=table&table=cash_book&id='.$id.'">'; else $logs='';

                   echo '<br><br><font color=grey>created by: '.$pers_name.', lastchange date: '.$logs_link.$lastchange.'</a>, login: '.$pers_last_change.'</font>';
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
                   echo '<tr><td>Hotel</td><td align=right>'.$room_charge_select.' %</td><td align=right>'.$formatter->formatCurrency($hotel_amount, 'IDR').'</td></tr>';
                   echo '<tr><td>Scuba</td><td align=right>'.$scuba_perc.' %</td><td align=right>'.$formatter->formatCurrency($scuba_amount, 'IDR').'</td></tr>';
                   echo '<tr><td>Total</td><td align=right></td><td align=right>'.$formatter->formatCurrency($total_amount, 'IDR').'</td></tr>';
                   echo '</table></div>';
                   }
?>



<hr>
            <div class="pure-control-group">
                <p align=center>Settlements:</p>
            </div>

<?php

if(str_replace(',','',$pay_bank_part)+str_replace(',','',$pay_bank_deposit)>0)
    $value_settlements = str_replace(',','',$pay_bank_part)+str_replace(',','',$pay_bank_deposit);
else
    $value_settlements = $value;


?>


            <div class="pure-controls" align=center>
                <table class="pure-table pure-table-horizontal pure-table-striped">
                <?php if($action<>'add') echo '<tr><th colspan=6 align=left><a class="pure-button pure-button-primary" href="settlements.php?action=add&foreign_id='.$id.'&foreign_table=cash_book&value='.$value_settlements.'&date='.$date.'&desc='.$desc.'"><i class="fa fa-plus" aria-hidden="true"></i></a></th></tr>'; ?>
                
                <tr><td>Value date</td><td>Account</td><td>Type</td><td>Description</td><td>Value</td><td></td></tr>


<?php
                    $stmt = $dbh->prepare("SELECT settlements.*, bank_statement.*
                    FROM settlements, bank_statement
                    WHERE settlements.foreign_id=".$id."
                    and settlements.bank_statement_id=bank_statement.id_bank_statement
                    and settlements.foreign_table='cash_book'
                    ORDER BY bank_statement.value_date DESC
                    ");
                    $stmt -> execute();
                    $last_change = $stmt->fetchAll();  

                    foreach($last_change as $row)
                            {
                                echo '<tr><td>'.$row['value_date'].'</td><td>'.$row['account_no'].'</td><td>'.$row['type'].'</td><td>'.$row['description'].'</td><td align="right">'.$formatter->formatCurrency($row['value'], 'IDR').'</td><td><a href="settlements.php?action=del&id='.$row['id_settlements'].'"><i class="far fa-trash-alt" aria-hidden="true"></i></a></td></tr>';

                                $sum_settlement += $row['value'];
                                }
    
                                echo '<tr><td colspan=4 align="right">SUM</td><td align="right">'.$formatter->formatCurrency($sum_settlement, 'IDR').'</td><td></td></tr>';
                                ?>
                </table>
            </div>




</div>






<script type="text/javascript">

function findSumAct(){
    var arr = document.getElementsByName('price[]');
    var tot=0;
    for(var i=0;i<arr.length;i++){
        if(parseInt(arr[i].value.split(",").join("")))
            tot += parseInt(arr[i].value.split(",").join(""));
    }
    // bylo ale nie uzywamy discount document.getElementById('value_before_discount').value = tot;
    // bylo ale nie uzywamy discount document.getElementById('total_act').value = tot-tot*document.getElementById('discount').value/100;

    document.getElementById('total_act').value = tot;


findSumInstructor();
findSumBooker();
findStillToPay();
}


function findSumPayment(){
    var tot=0;
    var cash_part=parseInt(document.getElementById('cash_part').value.split(",").join("")) || 0;
    var cash_deposit=parseInt(document.getElementById('cash_deposit').value.split(",").join("")) || 0;
    var bank_part=parseInt(document.getElementById('pay_bank_part').value.split(",").join("")) || 0;
    var bank_deposit=parseInt(document.getElementById('pay_bank_deposit').value.split(",").join("")) || 0;
    var creditcard=parseInt(document.getElementById('creditcard').value.split(",").join("")) || 0;
    var paypal=parseInt(document.getElementById('paypal').value.split(",").join("")) || 0;
    var roomcharge=parseInt(document.getElementById('roomcharge').value.split(",").join("")) || 0;

    tot = tot + cash_part;
    tot = tot + cash_deposit;
    tot = tot +  creditcard;
    tot = tot + bank_part;
    tot = tot + bank_deposit;
    tot = tot +  paypal;
    tot = tot +  roomcharge;

    document.getElementById('total_payment').value = tot;

    findStillToPay();
}


function findSumInstructor(){
    var tot=0;
    var instr_perc=parseInt(document.getElementById('instructor_perc').value.split(",").join("")) || 0;
    var total_act=parseInt(document.getElementById('total_act').value.split(",").join("")) || 0;

    tot = total_act * instr_perc/100;


    document.getElementById('instructor_commission').value = tot;
}

function findSumBooker(){
    var tot=0;
    var booker_perc=parseInt(document.getElementById('booker_perc').value.split(",").join("")) || 0;
    var total_act=parseInt(document.getElementById('total_act').value.split(",").join("")) || 0;

    tot = total_act * booker_perc/100;


    document.getElementById('booker_commission').value = tot;
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

    $("#instructor_id option").filter(function() {
        return $(this).val() == $("#instructor_perc").val();
    }).attr('selected', true);


    $("#instructor_id").on("change", function() {

        $("#instructor_perc").val($(this).find("option:selected").attr("value").toString().split('|')[1]);

    findSumInstructor();
    });
});


</script>
