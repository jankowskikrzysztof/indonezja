<?php


$id = $_GET['id'];
$back = $_GET['back'];
$sel_trip = $_GET['sel_trip'];


if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT client.*
                              FROM client
                              WHERE client.id_client='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $name_value = $edit_array[0][name];
                   $date_of_born_value = $edit_array[0][date_of_born];
                   $gender_value = $edit_array[0][gender];
                   $nationality_value = $edit_array[0][nationality];
                   $no_of_dives_value = $edit_array[0][no_of_dives];
                   $passport_value = $edit_array[0][passport];
                   $buyer_value = $edit_array[0][buyer];
                   $nitrox_value = $edit_array[0][nitrox];
                   $gear_to_rent_value = $edit_array[0][gear_to_rent];
                   $email_value = $edit_array[0][email];
                   $remarks_value = $edit_array[0][remarks];
                   $park_fee_value = $edit_array[0][park_fee];
                   }

?>



<div class=container>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >
<input type="hidden" name="back" value="<?php echo $back ?>" >
<input type="hidden" name="sel_trip" value="<?php echo $sel_trip ?>" >

            <div class="pure-control-group">
                <label for="name">Name</label>
                <input type="text" name="name" id="name" value="<?php echo $name_value; ?>" class="pure-input-1-4" autofocus>
            </div>

            <div class="pure-control-group">
                <label for="name">Date of born</label>
                <input type="text" name="date_of_born" id="date_of_born" value="<?php echo $date_of_born_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Gender</label>
                <input type="text" name="gender" id="gender" value="<?php echo $gender_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Nationality</label>
                <input type="text" name="nationality" id="nationality" value="<?php echo $nationality_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">No of dives</label>
                <input type="text" name="no_of_dives" id="no_of_dives" value="<?php echo $no_of_dives_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Passport</label>
                <input type="text" name="passport" id="passport" value="<?php echo $passport_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Buyer</label>
                <input type="text" name="buyer" id="buyer" value="<?php echo $buyer_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Nitrox</label>
                <input type="text" name="nitrox" id="nitrox" value="<?php echo $nitrox_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Gear to rent</label>
                <input type="text" name="gear_to_rent" id="gear_to_rent" value="<?php echo $gear_to_rent_value; ?>" class="pure-input-1-2">
            </div>

            <div class="pure-control-group">
                <label for="name">Remarks</label>
                <input type="text" name="remarks" id="remarks" value="<?php echo $remarks_value; ?>" class="pure-input-1-2">
            </div>

            <div class="pure-control-group">
                <label for="name">email</label>
                <input type="text" name="email" id="email" value="<?php echo $email_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-control-group">
                <label for="name">Park fee</label>
                <input type="text" name="park_fee" id="park_fee" value="<?php echo $park_fee_value; ?>" class="pure-input-1-4">
            </div>

            <div class="pure-controls">

                <?php
                if($_SESSION['permission']=='rw')
                   {
                   if($back<>'')
                       echo '<button type="submit" class="pure-button pure-button-primary">Submit & return</button>';
                   else
                       echo '<button type="submit" class="pure-button pure-button-primary">Submit</button>';

                   if($action<>'add')
                      echo '<a href="?action=del&id='.$id.'" class="pure-button button-error">Delete</a>';
                   }

                 ?>


            </div>
        </fieldset>
    </form>

<h2>Invoices</h2>

 <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
   <tr align=center>
     <th>Date</th>
     <th>Trip</th>
     <th>Booker</th>
     <th>Client</th>
     <th>Cabin/Bed</th>
     <th>Invoice No.</th>
     <th></th>
  </tr>
 </thead>

<?php


$sql_select = "SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
                              activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no, 
                              location_groups.name as loc_grp_name, DAY(cash_book.date) as dzien,

                              cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
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
                              cash_book.client_id

                              FROM (cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              left join booker on cash_book.booker_id=booker.id_booker
                              left join client on client.id_client=cash_book.client_id

                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              and cash_book.client_id='".$id."'

                              and location.id_location=14

                              and activity.id_activity=cash_book_item.activity_id
                              and cash_book_item.cash_book_id=cash_book.id_cash_book
                              and location.id_location=cash_book.location_id
                              and location.location_groups_id=location_groups.id_location_groups
                              GROUP BY cash_book.id_cash_book
                              ORDER BY cash_book.date desc";

//echo '<pre>'.$sql_select.'</pre>';

$stmt = $dbh->prepare($sql_select);

$stmt -> execute();

$row_array = $stmt->fetchAll();


foreach($row_array as $row)
    {
    $route_txt = '';
    if($row['boat_trip_id']>0)
       $route_txt = '<font style="font-size:80%;">'.$row['date_from'].' - '.$row['date_to'].'<br>'.$row['txt_route'].'</font>';

    $client_name = '';
    if($row['client_id']<>0)
       $client_name = '<a href="client.php?action=edit&id='.$row['client_id'].'">'.$row['client_name'].'</a>';

    echo '<tr>
    <td align=center>'.$row['date'].'</td>
    <td align=center>'.$route_txt.'</td>
    <td>'.$row['booker_name'].'</td>
    <td>'.$client_name.'</td>
    <td>'.$row['shortcut'].'</td>
    <td>'.$row['bill_no'].'</td>
    <td>';
	if($_SESSION['permission']=='rw' and $row['boat_trip_id']<>0)
		echo '<a href="empress_income.php?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit" aria-hidden="true"></i></a>';
	if($_SESSION['permission']=='rw' and $row['boat_trip_id']==0)
		echo '<a href="income.php?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit" aria-hidden="true"></i></a>';
    echo '</td>
    </tr>';
 
    $sum_value += $row['value'];
    $sum_total_pay += $row['total_pay'];
    $sum_still_to_pay += $still_to_pay;
 
    }

?>

</div>
