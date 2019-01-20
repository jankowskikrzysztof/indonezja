<?php

include('inc/calc_cur.inc');
$sys_currency = 'IDR';

if(isset($_POST["period"]))
  $period = $_POST["period"];
elseif(isset($_GET['period']))
$period = $_GET['period'];

$post_id = $_POST["id"];
$cash_book_group_id = $_POST["cash_book_group_id"];
$location_id = $_POST["location_id"];
$personel_id = $_SESSION["user_id"];
$instructor_id = explode("|",$_POST["instructor_id"]);
$client_id = $_POST["client_id"];

$activity_id = $_POST["activity_id"];
$boat_trip_id = $_POST["boat_trip_id"];
$pcs_no = $_POST["pcs_no"];
$price = $_POST["price"];
$item_currency = $_POST["item_currency"];
$item_currency_second = $_POST["item_currency_second"];
$id_cash_book_item = $_POST["id_cash_book_item"];

$activity_count = count($activity_id);

$type = $_POST["type"];
$type_cost = $_POST["type_cost"];
$date = $_POST["date"];
$bill_no = $_POST["bill_no"];
$desc = $_POST["desc"];
$value = str_replace(',','',$_POST["total_act"]);
$value_currency = str_replace(',','',$_POST["total_act_currency"]);
//$discount = $_POST["discount"];
//$value_before_discount = str_replace(',','',$_POST["value_before_discount"]);
$value_before_discount = str_replace(',','',$_POST["value_before_discount"]);
$value_before_discount_currency = str_replace(',','',$_POST["value_before_discount_currency"]);


$instructor = $_POST["instructor"];
$instructor_perc = $_POST["instructor_perc"];
$instructor_commission = str_replace(',','',$_POST["instructor_commission"]);
$instructor_commission_currency = str_replace(',','',$_POST["instructor_commission_currency"]);
$booker_id = $_POST["booker_id"];
$booker_perc = $_POST["booker_perc"];
$booker_commission = str_replace(',','',$_POST["booker_commission"]);
$booker_commission_currency = str_replace(',','',$_POST["booker_commission_currency"]);
$booking_no = $_POST["booking_no"];

//$pay_cash = str_replace(',','',$_POST["cash"]);


$pay_deposit_perc = str_replace(',','',$_POST["pay_deposit_perc"]);

$pay_cash_deposit = str_replace(',','',$_POST["pay_cash_deposit"]);
$pay_cash_deposit2 = str_replace(',','',$_POST["pay_cash_deposit2"]);
$pay_cash_deposit3 = str_replace(',','',$_POST["pay_cash_deposit3"]);
$pay_cash_deposit4 = str_replace(',','',$_POST["pay_cash_deposit4"]);
$pay_cash_part = str_replace(',','',$_POST["cash_part"]);
$pay_cash = $pay_cash_deposit + $pay_cash_deposit2 + $pay_cash_deposit3 + $pay_cash_deposit4 + $pay_cash_part;

$pay_cash_deposit_currency = str_replace(',','',$_POST["pay_cash_deposit_currency"]);
$pay_cash_deposit2_currency = str_replace(',','',$_POST["pay_cash_deposit2_currency"]);
$pay_cash_deposit3_currency = str_replace(',','',$_POST["pay_cash_deposit3_currency"]);
$pay_cash_deposit4_currency = str_replace(',','',$_POST["pay_cash_deposit4_currency"]);
$pay_cash_part_currency = str_replace(',','',$_POST["cash_part_currency"]);
$pay_cash_currency = $pay_cash_deposit_currency + $pay_cash_deposit2_currency + $pay_cash_deposit3_currency + $pay_cash_deposit4_currency + $pay_cash_part_currency;


$date_pay_deposit = str_replace(',','',$_POST["date_pay_deposit"]);
$date_pay_deposit2 = str_replace(',','',$_POST["date_pay_deposit2"]);
$date_pay_deposit3 = str_replace(',','',$_POST["date_pay_deposit3"]);
$date_pay_deposit4 = str_replace(',','',$_POST["date_pay_deposit4"]);

$date_pay_deposit_currency = str_replace(',','',$_POST["date_pay_deposit_currency"]);
$date_pay_deposit2_currency = str_replace(',','',$_POST["date_pay_deposit2_currency"]);
$date_pay_deposit3_currency = str_replace(',','',$_POST["date_pay_deposit3_currency"]);
$date_pay_deposit4_currency = str_replace(',','',$_POST["date_pay_deposit4_currency"]);

$pay_roomcharge = str_replace(',','',$_POST["roomcharge"]);
$pay_creditcard = str_replace(',','',$_POST["creditcard"]);
$pay_paypal = str_replace(',','',$_POST["paypal"]);

$pay_roomcharge_currency = str_replace(',','',$_POST["roomcharge_currency"]);
$pay_creditcard_currency = str_replace(',','',$_POST["creditcard_currency"]);
$pay_paypal_currency = str_replace(',','',$_POST["paypal_currency"]);

$pay_bank_deposit = str_replace(',','',$_POST["pay_bank_deposit"]);
$pay_bank_deposit2 = str_replace(',','',$_POST["pay_bank_deposit2"]);
$pay_bank_deposit3 = str_replace(',','',$_POST["pay_bank_deposit3"]);
$pay_bank_deposit4 = str_replace(',','',$_POST["pay_bank_deposit4"]);
$pay_bank_part = str_replace(',','',$_POST["pay_bank_part"]);
$pay_bank = $pay_bank_deposit + $pay_bank_deposit2 + $pay_bank_deposit3 + $pay_bank_deposit4 + $pay_bank_part;

$pay_bank_deposit_currency = str_replace(',','',$_POST["pay_bank_deposit_currency"]);
$pay_bank_deposit2_currency = str_replace(',','',$_POST["pay_bank_deposit2_currency"]);
$pay_bank_deposit3_currency = str_replace(',','',$_POST["pay_bank_deposit3_currency"]);
$pay_bank_deposit4_currency = str_replace(',','',$_POST["pay_bank_deposit4_currency"]);
$pay_bank_part_currency = str_replace(',','',$_POST["pay_bank_part_currency"]);
$pay_bank_currency = $pay_bank_deposit_currency + $pay_bank_deposit2_currency + $pay_bank_deposit3_currency + $pay_bank_deposit4_currency + $pay_bank_part_currency;


/////////////////////////////////// ZAMIANA WALUT

//conv_currency($change_value,'to',USD,'2018-05-08',$dbh)

$currency = $_POST["currency"];

    for($i=0;$i<$activity_count;$i++)
       {
       if($currency <> $sys_currency)
          {
          $price_currency[$i] = str_replace(',','',$price[$i]);
          $price[$i] = conv_currency(str_replace(',','',$price[$i]),'from',$currency,$date,$dbh);
          $item_currency_second[$i] = $currency;
          $item_currency[$i] = $sys_currency;
          }
        }

       if($currency <> $sys_currency)
          {
          $booker_commission_currency = $booker_commission;
          $booker_commission = conv_currency($booker_commission,'from',$currency,$date,$dbh);

          $instructor_commission_currency = $instructor_commission;
          $instructor_commission = conv_currency($instructor_commission,'from',$currency,$date,$dbh);


          $value_currency = $value;
          $value = conv_currency($value,'from',$currency,$date,$dbh);

        echo "$value = conv_currency($value,'from',$currency,$date);";

          $value_before_discount_currency = $value_before_discount;
          $value_before_discount = conv_currency($value_before_discount,'from',$currency,$date,$dbh);



          $pay_roomcharge_currency = $pay_roomcharge;
          $pay_roomcharge = conv_currency($pay_roomcharge,'from',$currency,$date,$dbh);

          $pay_creditcard_currency = $pay_creditcard;
          $pay_creditcard = conv_currency($pay_creditcard,'from',$currency,$date,$dbh);

          $pay_paypal_currency = $pay_paypal;
          $pay_paypal = conv_currency($pay_paypal,'from',$currency,$date,$dbh);


          $pay_bank_part_currency = $pay_bank_part;
          $pay_bank_part = conv_currency($pay_bank_part,'from',$currency,$date,$dbh);

          $pay_bank_currency = $pay_bank;
          $pay_bank = conv_currency($pay_bank,'from',$currency,$date,$dbh);

          $pay_bank_deposit_currency = $pay_bank_deposit;
          $pay_bank_deposit = conv_currency($pay_bank_deposit,'from',$currency,$date_pay_deposit,$dbh);

          $pay_bank_deposit2_currency = $pay_bank_deposit2;
          $pay_bank_deposit2 = conv_currency($pay_bank_deposit2,'from',$currency,$date_pay_deposit2,$dbh);

          $pay_bank_deposit3_currency = $pay_bank_deposit3;
          $pay_bank_deposit3 = conv_currency($pay_bank_deposit3,'from',$currency,$date_pay_deposit3,$dbh);

          $pay_bank_deposit4_currency = $pay_bank_deposit4;
          $pay_bank_deposit4 = conv_currency($pay_bank_deposit4,'from',$currency,$date_pay_deposit4,$dbh);



          $pay_cash_part_currency = $pay_cash_part;
          $pay_cash_part = conv_currency($pay_cash_part,'from',$currency,$date,$dbh);

          $pay_cash_currency = $pay_cash;
          $pay_cash = conv_currency($pay_cash,'from',$currency,$date,$dbh);

          $pay_cash_deposit_currency = $pay_cash_deposit;
          $pay_cash_deposit = conv_currency($pay_cash_deposit,'from',$currency,$date_pay_deposit,$dbh);

          $pay_cash_deposit2_currency = $pay_cash_deposit2;
          $pay_cash_deposit2 = conv_currency($pay_cash_deposit2,'from',$currency,$date_pay_deposit2,$dbh);

          $pay_cash_deposit3_currency = $pay_cash_deposit3;
          $pay_cash_deposit3 = conv_currency($pay_cash_deposit3,'from',$currency,$date_pay_deposit3,$dbh);

          $pay_cash_deposit4_currency = $pay_cash_deposit4;
          $pay_cash_deposit4 = conv_currency($pay_bank_deposit4,'from',$currency,$date_pay_deposit4,$dbh);


          $currency_second = $currency;
          $currency = $sys_currency;
          }

/////////////////////////////////// ZAMIANA WALUT



if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $insert = "INSERT INTO `cash_book`
                                  (`cash_book_group_id`,
                                   `location_id`,
                                   `personel_id`,
                                   `client_id`,
                                   `boat_trip_id`,
                                   `type`,
                                   `type_cost`,
                                   `date`,
                                   `bill_no`,
                                   `desc`,
                                   `value`,
                                   `value_currency`,
                                   `currency`,
                                   `currency_second`,
                                   `discount`,
                                   `discount_currency`,
                                   `value_before_discount`,
                                   `value_before_discount_currency`,
                                   `instructor_id`,
                                   `instructor_perc`,
                                   `instructor_commission`,
                                   `instructor_commission_currency`,
                                   `booker_id`,
                                   `booker_perc`,
                                   `booker_commission`,
                                   `booker_commission_currency`,
                                   `booking_no`,
                                   `cash_report_only`,
                                   `pay_deposit_perc`,
                                   `pay_cash`,
                                   `pay_cash_currency`,
                                   `pay_cash_part`,
                                   `pay_cash_part_currency`,
                                   `pay_cash_deposit`,
                                   `pay_cash_deposit_currency`,
                                   `date_pay_deposit`,
                                   `pay_cash_deposit2`,
                                   `pay_cash_deposit2_currency`,
                                   `date_pay_deposit2`,
                                   `pay_cash_deposit3`,
                                   `pay_cash_deposit3_currency`,
                                   `date_pay_deposit3`,
                                   `pay_cash_deposit4`,
                                   `pay_cash_deposit4_currency`,
                                   `date_pay_deposit4`,
                                   `pay_bank_part`,
                                   `pay_bank_part_currency`,
                                   `pay_bank_deposit`,
                                   `pay_bank_deposit_currency`,
                                   `pay_bank_deposit2`,
                                   `pay_bank_deposit2_currency`,
                                   `pay_bank_deposit3`,
                                   `pay_bank_deposit3_currency`,
                                   `pay_bank_deposit4`,
                                   `pay_bank_deposit4_currency`,
                                   `pay_roomcharge`,
                                   `pay_roomcharge_currency`,
                                   `pay_creditcard`,
                                   `pay_creditcard_currency`,
                                   `pay_paypal`,
                                   `pay_paypal_currency`,
                                   `pay_bank`,
                                   `pay_bank_currency`
                                    )

                           VALUES ('".$cash_book_group_id."',
                                   '".$location_id."',
                                   '".$personel_id."',
                                   '".$client_id."',
                                   '".$boat_trip_id."',
                                   '".$type."',
                                   '".$type_cost."',
                                   '".$date."',
                                   '".$bill_no."',
                                   '".$desc."',
                                   '".$value."',
                                   '".$value_currency."',
                                   '".$currency."',
                                   '".$currency_second."',
                                   '".$discount."',
                                   '".$discount_currency."',
                                   '".$value_before_discount."',
                                   '".$value_before_discount_currency."',
                                   '".$instructor_id[0]."',
                                   '".$instructor_perc."',
                                   '".$instructor_commission."',
                                   '".$instructor_commission_currency."',
                                   '".$booker_id."',
                                   '".$booker_perc."',
                                   '".$booker_commission."',
                                   '".$booker_commission_currency."',
                                   '".$booking_no."',
                                   '0',
                                   '".$pay_deposit_perc."',
                                   '".$pay_cash."',
                                   '".$pay_cash_currency."',
                                   '".$pay_cash_part."',
                                   '".$pay_cash_part_currency."',
                                   '".$pay_cash_deposit."',
                                   '".$pay_cash_deposit_currency."',
                                   '".$date_pay_deposit."',
                                   '".$pay_cash_deposit2."',
                                   '".$pay_cash_deposit2_currency."',
                                   '".$date_pay_deposit2."',
                                   '".$pay_cash_deposit3."',
                                   '".$pay_cash_deposit3_currency."',
                                   '".$date_pay_deposit3."',
                                   '".$pay_cash_deposit4."',
                                   '".$pay_cash_deposit4_currency."',
                                   '".$date_pay_deposit4."',
                                   '".$pay_bank_part."',
                                   '".$pay_bank_part_currency."',
                                   '".$pay_bank_deposit."',
                                   '".$pay_bank_deposit_currency."',
                                   '".$pay_bank_deposit2."',
                                   '".$pay_bank_deposit2_currency."',
                                   '".$pay_bank_deposit3."',
                                   '".$pay_bank_deposit3_currency."',
                                   '".$pay_bank_deposit4."',
                                   '".$pay_bank_deposit4_currency."',
                                   '".$pay_roomcharge."',
                                   '".$pay_roomcharge_currency."',
                                   '".$pay_creditcard."',
                                   '".$pay_creditcard_currency."',
                                   '".$pay_paypal."',
                                   '".$pay_paypal_currency."',
                                   '".$pay_bank."',
                                   '".$pay_bank_currency."'
                                   )";

    $stmt = $dbh->prepare($insert);


    $stmt -> execute();

    //echo '<br><br><pre>'.$insert.'<br><br>';

    //$stmt->debugDumpParams();

    //echo '<br><br>';

    //print_r($stmt->errorInfo());
    $id = $dbh->lastInsertId();
    $last_insert_id = $dbh->lastInsertId();

    for($i=0;$i<$activity_count;$i++)
       {
       //echo '<br>'.$i.'<br>';
       $insert = "INSERT INTO `cash_book_item`
                                  (`cash_book_id`,
                                   `activity_id`,
                                   `amount`,
                                   `value`,
                                   `value_currency`,
                                   `item_currency`,
                                   `item_currency_second`
                                   )

                           VALUES ('".$id."',
                                   '".$activity_id[$i]."',
                                   '".$pcs_no[$i]."',
                                   '".str_replace(',','',$price[$i])."',
                                   '".str_replace(',','',$price_currency[$i])."',
                                   '".$item_currency[$i]."',
                                   '".$item_currency_second[$i]."'
                                   )";
        //echo $insert.'<br>';
        $stmt = $dbh->prepare($insert);
        $stmt -> execute();
       }


    echo '<p>Data '.$id.' add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {
        
    $stmt = $dbh->prepare("UPDATE `cash_book` SET

                           `date` = '".$date."',
                           `bill_no` = '".$bill_no."',
                           `desc` = '".$desc."',
                           `value` = '".$value."',
                           `value_currency` = '".$value_currency."',
                           `currency` = '".$currency."',
                           `currency_second` = '".$currency_second."',
                           `discount` = '".$discount."',
                           `discount_currency` = '".$discount_currency."',
                           `value_before_discount` = '".$value_before_discount."',
                           `value_before_discount_currency` = '".$value_before_discount_currency."',
                           `location_id` = '".$location_id."',
                           `client_id` = '".$client_id."',
                           `boat_trip_id` = '".$boat_trip_id."',

                           `instructor_id` = '".$instructor_id[0]."',
                           `instructor_perc` = '".$instructor_perc."',
                           `instructor_commission` = '".$instructor_commission."',
                           `instructor_commission_currency` = '".$instructor_commission_currency."',
                           `booker_id` = '".$booker_id."',
                           `booker_perc` = '".$booker_perc."',
                           `booker_commission` = '".$booker_commission."',
                           `booker_commission_currency` = '".$booker_commission_currency."',
                           `booking_no` = '".$booking_no."',
                           `cash_report_only` = '0',
                           `pay_deposit_perc` = '".$pay_deposit_perc."',
                           `pay_cash` = '".$pay_cash."',
                           `pay_cash_currency` = '".$pay_cash_currency."',
                           `pay_cash_deposit` = '".$pay_cash_deposit."',
                           `pay_cash_deposit_currency` = '".$pay_cash_deposit_currency."',
                           `pay_cash_deposit2` = '".$pay_cash_deposit2."',
                           `pay_cash_deposit2_currency` = '".$pay_cash_deposit2_currency."',
                           `pay_cash_deposit3` = '".$pay_cash_deposit3."',
                           `pay_cash_deposit3_currency` = '".$pay_cash_deposit3_currency."',
                           `pay_cash_deposit4` = '".$pay_cash_deposit4."',
                           `pay_cash_deposit4_currency` = '".$pay_cash_deposit4_currency."',
                           `date_pay_deposit` = '".$date_pay_deposit."',
                           `date_pay_deposit2` = '".$date_pay_deposit2."',
                           `date_pay_deposit3` = '".$date_pay_deposit3."',
                           `date_pay_deposit4` = '".$date_pay_deposit4."',
                           `pay_cash_part` = '".$pay_cash_part."',
                           `pay_cash_part_currency` = '".$pay_cash_part_currency."',
                           `pay_bank_part` = '".$pay_bank_part."',
                           `pay_bank_part_currency` = '".$pay_bank_part_currency."',
                           `pay_bank_deposit` = '".$pay_bank_deposit."',
                           `pay_bank_deposit_currency` = '".$pay_bank_deposit_currency."',
                           `pay_bank_deposit2` = '".$pay_bank_deposit2."',
                           `pay_bank_deposit2_currency` = '".$pay_bank_deposit2_currency."',
                           `pay_bank_deposit3` = '".$pay_bank_deposit3."',
                           `pay_bank_deposit3_currency` = '".$pay_bank_deposit3_currency."',
                           `pay_bank_deposit4` = '".$pay_bank_deposit4."',
                           `pay_bank_deposit4_currency` = '".$pay_bank_deposit4_currency."',
                           `pay_roomcharge` = '".$pay_roomcharge."',
                           `pay_roomcharge_currency` = '".$pay_roomcharge_currency."',
                           `pay_creditcard` = '".$pay_creditcard."',
                           `pay_creditcard_currency` = '".$pay_creditcard_currency."',
                           `pay_paypal` = '".$pay_paypal."',
                           `pay_paypal_currency` = '".$pay_paypal_currency."',
                           `pay_bank` = '".$pay_bank."',
                           `pay_bank_currency` = '".$pay_bank_currency."'



                           WHERE `cash_book`.`id_cash_book` = '".$post_id."'
                           ");

    $stmt -> execute();



    for($i=0;$i<$activity_count;$i++)
       {
       $stmt = $dbh->prepare("UPDATE `cash_book_item` SET

                                   `activity_id` = '".$activity_id[$i]."',
                                   `amount` =  '".$pcs_no[$i]."',
                                   `value` =  '".str_replace(',','',$price[$i])."',
                                   `value_currency` =  '".str_replace(',','',$price_currency[$i])."',
                                   `item_currency` =  '".$item_currency[$i]."',
                                   `item_currency_second` =  '".$item_currency_second[$i]."'


                              WHERE `cash_book_item`.`id_cash_book_item` = '".$id_cash_book_item[$i]."'
                           ");
         $stmt -> execute();
       }



    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['location_id_limit']==0 and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `cash_book` WHERE `cash_book`.`id_cash_book` = '".$post_id."' ");

    $stmt -> execute();

    //$err_list .= $stmt->error;
    //$row_count = $stmt->rowCount();

    $stmt = $dbh->prepare("DELETE FROM `cash_book_item` WHERE `cash_book_item`.`cash_book_id` = '".$post_id."' ");

    $stmt -> execute();

    //$err_list .= $stmt->error;

    echo '<p>Data delete success</p>';
    }


$target_table = 'cash_book';
$txt = 'income';


if($_POST['source_action']=='add')
    $target_id = $last_insert_id;
else
    $target_id = $post_id;

if($_GET['action']=='del')
    $target_action = $_GET['action'];
else
    $target_action = $_POST['source_action'];

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`target_table`,`target_id`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$target_action."',
             '".$target_table."',
             '".$target_id."',
             '".$txt."'
             )");

$stmt -> execute();


//echo 'header(location: empress_income.php?period='.$period.');';
//echo '<br>'.$row_count.' a: '.$err_list;

//header('location: empress_income.php?period='.$period);
//exit;


?>

