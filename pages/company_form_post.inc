<?php

$post_id = $_POST[id];
$name = $_POST[name];
$address = $_POST[address];





if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `company`
                                  (`name`,`address`,`code`,`city`,`nation`,`identification_number`)
                           VALUES ('".$name."','".$address."','".$code."','".$city."','".$nation."','".$identification_number."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    $stmt = $dbh->prepare("UPDATE `company` SET

                           `name` = '".$name."',
                           `address` = '".$address."'
                           `code` = '".$code."'
                           `city` = '".$city."'
                           `nation` = '".$nation."'
                           `identification_number` = '".$identification_number."'

                           WHERE `company`.`id_company` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `company` WHERE `company`.`id_company` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'company id=".$post_id."'
             )");
$stmt -> execute();

header('location: company.php');
exit;

?>

