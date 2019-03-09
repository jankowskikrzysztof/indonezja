<?php

/*<script type="text/javascript" src="inc/bootstrap-filestyle-2.1.0/src/bootstrap-filestyle.min.js"> 

</script>
<script type="text/javascript"> 
$(":file").filestyle({text: "Find file"});
</script>
*/



$stmt = $dbh->prepare("SELECT *
                              FROM personel
                              ORDER BY personel.name");
$stmt -> execute();
$personel_array = $stmt->fetchAll();


if($personel_location_selected <> '' and $action<>'edit')
$def_location = 'and location.id_location IN ('.$personel_location_selected.')';
else
$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

      //$personel_location_selected <> '' and 
if($action<>'edit')
      $cb_location_limit = 'and (cash_book_group.location_id_limit LIKE \'%\"'.$personel_location_selected.'\"%\' or cash_book_group.location_id_limit=0 or cash_book_group.location_id_limit="")';
else
      $cb_location_limit = '';




$stmt = $dbh->prepare("SELECT location.name as loc_name,location.id_location
                              FROM location
                              WHERE 1=1
                              ".$def_location."
                              ".$location_limit."
                              ORDER BY location.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();

//                              and cash_book_group.id_cash_book_group NOT IN (21,22,24,3)

$stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, id_boat_trip as boat_trip_id
                              FROM boat_trip
                             
                              ORDER BY date_to");
$stmt -> execute();
$boat_trip_array = $stmt->fetchAll();
// WHERE YEAR(date_to)=".$period_year."

/*
echo "SELECT cash_book_group.name as cb_name,cash_book_group.id_cash_book_group
                              FROM cash_book_group
                              WHERE cash_book_group.type=2

                              ".$cb_location_limit."
                              ORDER BY cash_book_group.name";
*/

$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name,cash_book_group.id_cash_book_group
                              FROM cash_book_group
                              WHERE cash_book_group.type=2
                              and cash_book_group.only_cash<>1
                              ".$cb_location_limit."
                              ORDER BY cash_book_group.name");
$stmt -> execute();
$cash_book_group_array = $stmt->fetchAll();


$stmt = $dbh->prepare("SELECT currency.currency
                              FROM currency
                              ORDER BY currency.id_currency");
$stmt -> execute();
$currency_array = $stmt->fetchAll();


$id = $_GET['id'];
$date_now = date("Y-m-d");

if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, cash_book.location_id, cash_book.personel_id,
                                                 personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value as price, cash_book.currency,
                              cash_book.id_cash_book, cash_book.cash_book_group_id, cash_book.date, cash_book.costs_bill_value,
                              cash_book.scan_org_filename_ext,
                              boat_trip.date_from, boat_trip.date_to, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,

                              cash_book.type_payment,
                              cash_book.lastchange



                              FROM (cash_book, personel, location, cash_book_group)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              and cash_book.id_cash_book='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $location_select = $edit_array[0]['location_id'];
                   $boat_trip_select = $edit_array[0]['boat_trip_id'];
                   $personel_select = $edit_array[0]['personel_id'];
                   $cash_book_group_select = $edit_array[0]['cash_book_group_id'];
                   $desc = $edit_array[0]['desc'];
                   $date = $edit_array[0]['date'];
                   $type_payment = $edit_array[0]['type_payment'];
                   $costs_bill_value = $edit_array[0]['costs_bill_value'];
                   $price = $edit_array[0]['price'];
                   $scan_org_filename_ext = $edit_array[0]['scan_org_filename_ext'];

                   $lastchange = $edit_array[0]['lastchange'];

                   }

if($action=='add')
                   {
                   $cash_book_group_select = '';
                   $location_select = '';
                   $desc = '';
                   $costs_bill_value = '';
                   $price = '';
                   $date = date('Y-m-d');

                   $id = '';
                   }


if($_GET['datt']==1 and $_SESSION['permission']=='rw')
    {
    rename('img_uploads/costs/costs_'.$id.'.'.$scan_org_filename_ext,'img_uploads/costs/costs_'.$id.'_'.uniqid().'.'.$scan_org_filename_ext);
    }


?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST" enctype="multipart/form-data">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >
<input type="hidden" name="type" value="2" >



            <div class="pure-control-group">
                <label for="name">Date</label>
                <input type="date" name="date" id="date" value="<?php echo $date; ?>" class="pure-input-1-1">
            </div>


            <div class="pure-control-group">
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

           <?php
            if($personel_location_selected == 14)
               { ?>
            <div class="pure-control-group">
                <label for="location_id">Empress trip</label>
                <select name="boat_trip_id" id="boat_trip_id" class="pure-input-1-1">
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

                        if( $boat_trip_select == $row['boat_trip_id'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['boat_trip_id'].'" '.$sel.$option_bgcolor.'>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</option>';
                        }
                ?>
                </select>
            </div>

            <?php
                 }
            elseif($personel_location_selected == '')
                {
                ?>
            <div class="pure-control-group">
                <label for="location_id">Empress trip</label>
                <font style="font-size:70%;" color=grey>no trip selection on "all" location - choose Empress at top</font>
            </div>
            <?php
                }
                 ?>


            <div class="pure-control-group">
                <label for="cash_book_group_id">Cost Group</label>
                <select name="cash_book_group_id" id="cash_book_group_id" class="pure-input-1-1" autofocus>
                <?php
                     foreach($cash_book_group_array as $row)
                        {
                        if( $cash_book_group_select == $row['id_cash_book_group'] ) $sel='selected'; else $sel = '';

                        echo '<option value="'.$row['id_cash_book_group'].'" '.$sel.'>'.$row['cb_name'].'</option>';
                        }
                ?>
                </select>
   </div>


        <div class="pure-control-group">
                <label for="type_payment">Payment method</label>
                <select name="type_payment" id="type_payment" class="pure-input-1-1">


                    <option value="0" <?php if($type_payment==0) echo 'selected'?>>Cash</option>
                    <option value="2" <?php if($type_payment==2) echo 'selected'?>>Bank transfer</option>
                    <option value="3" <?php if($type_payment==3) echo 'selected'?>>PayPal</option>

                </select>
   </div>


  <div class="pure-control-group">
                <label for="desc">Description</label>
                <input type="text" name="desc" id="description" value='<?php echo $desc; ?>' class="pure-input-1-1">
     </div>

  <!--<div class="pure-control-group">
                <label for="costs_bill_value">Bill Amount</label>
                <input type="tex" name="costs_bill_value" id="costs_bill_value" value="<?php echo str_replace('.00','',str_replace(',','',$costs_bill_value)); ?>" class="money" readonly>
     </div>
   -->
   
  <div class="pure-control-group">
                <label for="price">Bill Paid</label>
                <input type="text" name="price" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$price)); ?>" class="money">
            </div>

<!--
blokada wpisywania kosztow nie zaplaconych
  <div class="pure-control-group">
                <label for="price">Amount Paid</label>
                <input type="text" name="price" id="price" value="<?php echo str_replace('.00','',str_replace(',','',$price)); ?>" class="money">
            </div>

              <div class="pure-control-group">

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


<?php 
//if($_SESSION['user_id'] == 1 and $action=='edit')
if($action=='edit')
    { 
    ?>

  <div class="pure-control-group">
                <label for="price">Document scan</label>
                
<?php

    if(file_exists('img_uploads/costs/costs_'.$id.'.'.$scan_org_filename_ext.''))
        {
        if(in_array($scan_org_filename_ext,array('jpg','jpeg','png')))
            $img_file = '<img src="img_uploads/costs/costs_'.$id.'.'.$scan_org_filename_ext.'" style="max-width:200px">';
        else
            $img_file = '<i class="far fa-file-pdf fa-5x"></i>';

        echo '<a href="img_uploads/costs/costs_'.$id.'.'.$scan_org_filename_ext.'" target=_blank>'.$img_file.'</a> &nbsp;&nbsp;&nbsp;
        <a href="?action=edit&id='.$id.'&datt=1"><i class="fas fa-times"></i></a>';
        }
    else
        {
        echo '<input type="file" name="fileToUpload" id="fileToUpload">';
        }

    echo '</div>';


    }; ?>


            <div class="pure-controls">




                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button>';

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


                   echo '<br><br><font color=grey>lastchange date: '.$lastchange.', login: '.$pers_last_change.'</font>';
                   }

                 ?>

            </div>
        </fieldset>
    </form>

<hr>

<?php
$value = $price;
?>

            <div class="pure-control-group">
                <p align=center>Settlements:</p>
            </div>

            <div class="pure-controls" align=center>
                <table class="pure-table pure-table-horizontal pure-table-striped">
                
                <?php if($action<>'add') echo '<tr><th colspan=6 align=left><a class="pure-button pure-button-primary" href="settlements.php?action=add&foreign_id='.$id.'&foreign_table=cash_book&value='.$value.'&date='.$date.'&desc='.$desc.'"><i class="fa fa-plus" aria-hidden="true"></i></a></th></tr>'; ?>
                
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
