<?php

$post_id = $_POST['id'];
$cash_book_group_id = $_POST['cash_book_group_id'];
$location_id = $_POST['location_id'];
$personel_id = $_SESSION['user_id'];
$instructor_id = explode("|",$_POST['instructor_id']);

$activity_id = $_POST['activity_id'];
$pcs_no = $_POST['pcs_no'];
$price = $_POST['price'];
$id_cash_book_item = $_POST['id_cash_book_item'];

$activity_count = count($activity_id);

$type = $_POST['type'];
$date = $_POST['date'];
$bill_no = $_POST['bill_no'];
$desc = $_POST['desc'];
$value = str_replace(',','',$_POST['total_act']);
//$discount = $_POST[discount];
//$value_before_discount = str_replace(',','',$_POST[value_before_discount]);
$value_before_discount = str_replace(',','',$_POST['total_act']);


$instructor = $_POST['instructor'];
$instructor_perc = $_POST['instructor_perc'];
$instructor_commission = str_replace(',','',$_POST['instructor_commission']);
$booker_id = $_POST['booker_id'];
$booker_perc = $_POST['booker_perc'];
$booker_commission = str_replace(',','',$_POST['booker_commission']);


//$pay_cash = str_replace(',','',$_POST[cash]);

$pay_cash_deposit = str_replace(',','',$_POST['cash_deposit']);
$pay_cash_part = str_replace(',','',$_POST['cash_part']);
$pay_cash = $pay_cash_deposit + $pay_cash_part;

$pay_roomcharge = str_replace(',','',$_POST['roomcharge']);
$pay_creditcard = str_replace(',','',$_POST['creditcard']);
//$pay_bank = str_replace(',','',$_POST[pay_bank]);
$pay_paypal = str_replace(',','',$_POST['paypal']);

$pay_bank_deposit = str_replace(',','',$_POST['pay_bank_deposit']);
$pay_bank_part = str_replace(',','',$_POST['pay_bank_part']);
$pay_bank = $pay_bank_deposit + $pay_bank_part;


$currency = $_POST['currency'];

$target_dir = "img_uploads/income/";
$target_file = $target_dir . 'income_' .$_POST['id'];
$uploadOk = 1;


if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $insert = "INSERT INTO `cash_book`
                                  (`cash_book_group_id`,
                                   `location_id`,
                                   `personel_id`,
                                   `type`,
                                   `date`,
                                   `bill_no`,
                                   `desc`,
                                   `value`,
                                   `currency`,
                                   `discount`,
                                   `value_before_discount`,
                                   `instructor_id`,
                                   `instructor_perc`,
                                   `instructor_commission`,
                                   `booker_id`,
                                   `booker_perc`,
                                   `booker_commission`,
                                   `cash_report_only`,
                                   `pay_cash`,
                                   `pay_cash_part`,
                                   `pay_cash_deposit`,
                                   `pay_roomcharge`,
                                   `pay_creditcard`,
                                   `pay_paypal`,
                                   `pay_bank_part`,
                                   `pay_bank_deposit`,
                                   `pay_bank`)

                           VALUES ('".$cash_book_group_id."',
                                   '".$location_id."',
                                   '".$personel_id."',
                                   '".$type."',
                                   '".$date."',
                                   '".$bill_no."',
                                   '".$desc."',
                                   '".$value."',
                                   '".$currency."',
                                   '".$discount."',
                                   '".$value_before_discount."',
                                   '".$instructor_id[0]."',
                                   '".$instructor_perc."',
                                   '".$instructor_commission."',
                                   '".$booker_id."',
                                   '".$booker_perc."',
                                   '".$booker_commission."',
                                   '0',
                                   '".$pay_cash."',
                                   '".$pay_cash_part."',
                                   '".$pay_cash_deposit."',
                                   '".$pay_roomcharge."',
                                   '".$pay_creditcard."',
                                   '".$pay_paypal."',
                                   '".$pay_bank_part."',
                                   '".$pay_bank_deposit."',
                                   '".$pay_bank."')";

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
       $stmt = $dbh->prepare("INSERT INTO `cash_book_item`
                                  (`cash_book_id`,
                                   `activity_id`,
                                   `amount`,
                                   `value`)

                           VALUES ('".$id."',
                                   '".$activity_id[$i]."',
                                   '".$pcs_no[$i]."',
                                   '".str_replace(',','',$price[$i])."')
                           ");
         $stmt -> execute();
       }


    echo '<p>Data '.$id.' add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
        echo "Sorry, your file was not uploaded.";
    // if everything is ok, try to upload file
    } else {
        
        $file_ext = pathinfo(parse_url($_FILES["fileToUpload"]["name"])['path'], PATHINFO_EXTENSION);
        if($file_ext <> "")
            $file_info_sql = '
                            `scan_org_filename` = "'.$_FILES['fileToUpload']['name'].'",
                            `scan_org_filename_ext` = "'.$file_ext.'",';
        else
            $file_info_sql = '';

        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file.'.'.$file_ext)) {
            echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
            
        } elseif($_FILES["fileToUpload"]<>NULL) {
            echo "<font color=red>Sorry, there was an error uploading your file.</font>";
        }
    }

    $stmt = $dbh->prepare("UPDATE `cash_book` SET

                           `date` = '".$date."',
                           `bill_no` = '".$bill_no."',
                           `desc` = '".$desc."',
                           `value` = '".$value."',
                           `currency` = '".$currency."',
                           `discount` = '".$discount."',
                           `value_before_discount` = '".$value_before_discount."',
                           `location_id` = '".$location_id."',

                           `instructor_id` = '".$instructor_id[0]."',
                           `instructor_perc` = '".$instructor_perc."',
                           `instructor_commission` = '".$instructor_commission."',
                           `booker_id` = '".$booker_id."',
                           `booker_perc` = '".$booker_perc."',
                           `booker_commission` = '".$booker_commission."',
                           `cash_report_only` = '0',
                           `pay_cash` = '".$pay_cash."',
                           `pay_cash_deposit` = '".$pay_cash_deposit."',
                           `pay_cash_part` = '".$pay_cash_part."',
                           `pay_bank_deposit` = '".$pay_bank_deposit."',
                           `pay_bank_part` = '".$pay_bank_part."',
                           `pay_roomcharge` = '".$pay_roomcharge."',
                           `pay_creditcard` = '".$pay_creditcard."',
                           `pay_paypal` = '".$pay_paypal."',
                           `pay_bank` = '".$pay_bank."'



                           WHERE `cash_book`.`id_cash_book` = '".$post_id."'
                           ");

    $stmt -> execute();



    for($i=0;$i<$activity_count;$i++)
       {
       $stmt = $dbh->prepare("UPDATE `cash_book_item` SET

                                   `activity_id` = '".$activity_id[$i]."',
                                   `amount` =  '".$pcs_no[$i]."',
                                   `value` =  '".str_replace(',','',$price[$i])."'

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

    $stmt = $dbh->prepare("DELETE FROM `cash_book_item` WHERE `cash_book_item`.`cash_book_id` = '".$post_id."' ");

    $stmt -> execute();

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



header('location: income.php');
exit;


?>

