<?php


$post_id = $_POST['id'];
$cash_book_group_id = $_POST['cash_book_group_id'];
$location_id = $_POST['location_id'];
$boat_trip_id = $_POST['boat_trip_id'];
$personel_id = $_SESSION['user_id'];
$activity_id = 0;
$type = $_POST['type'];
$type_payment = $_POST['type_payment'];
$date = $_POST['date'];
$desc = addslashes($_POST['desc']);
//$costs_bill_value = str_replace(',','',$_POST['costs_bill_value']);
$value = str_replace(',','',$_POST['price']);
$costs_bill_value = $value;
//$value = str_replace(',','',$_POST['costs_bill_value']);
$currency = $_POST['currency'];

$target_dir = "img_uploads/costs/";
//$target_file = $target_dir . 'costs_' .$_POST['id'].'_'.basename($_FILES["fileToUpload"]["name"]);
$target_file = $target_dir . 'costs_' .$_POST['id'];
$uploadOk = 1;
//$imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));


if($_SESSION['user_id'] == 1)
    {
        
/* 
        echo '<pre>';

        var_dump($imageFileType);
echo '<hr>';

        var_dump($_FILES['fileToUpload']);

        echo '<hr>';

        var_dump($_FILES["fileToUpload"]["tmp_name"]);

        
echo '</pre>';
 */
    }


if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `cash_book`
                                  (`cash_book_group_id`,
                                   `location_id`,
                                   `personel_id`,
                                   `boat_trip_id`,
                                   `cash_report_only`,
                                   `type`,
                                   `type_payment`,
                                   `date`,
                                   `desc`,
                                   `costs_bill_value`,
                                   `value`,
                                   `currency`)

                           VALUES ('".$cash_book_group_id."',
                                   '".$location_id."',
                                   '".$personel_id."',
                                   '".$boat_trip_id."',
                                   '0',
                                   '".$type."',
                                   '".$type_payment."',
                                   '".$date."',
                                   '".$desc."',
                                   '".$costs_bill_value."',
                                   '".$value."',
                                   '".$currency."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';
    $last_insert_id = $dbh->lastInsertId();

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    // Check if $uploadOk is set to 0 by an error
    //if ($uploadOk == 0 and $_SESSION['user_id'] == 1) {
    if ($uploadOk == 0) {
        echo "Sorry, your file was not uploaded.";
    // if everything is ok, try to upload file
//} elseif($_SESSION['user_id'] == 1) {    
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

                           `desc` = '".$desc."',
                           `date` = '".$date."',
                           `boat_trip_id` = '".$boat_trip_id."',
                           `costs_bill_value` = '".$costs_bill_value."',
                           `value` = '".$value."',
                           `type_payment` = '".$type_payment."',
                           `currency` = '".$currency."',
                           `cash_report_only` = '0',
                           ".$file_info_sql."
                           `cash_book_group_id` = '".$cash_book_group_id."'
                           
                           

                           WHERE `cash_book`.`id_cash_book` = '".$post_id."'
                           ");


    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['location_id_limit']==0 and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `cash_book` WHERE `cash_book`.`id_cash_book` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }


$target_table = 'cash_book';
$txt = 'costs';


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


header('location: costs.php');
exit;

?>

