<?php

$post_id = $_POST['id'];
$name = $_POST['name'];
$commission = $_POST['commission'];
$company_id = $_POST['company_id'];





if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `booker`
                                  (`name`,`commission`,`company_id`)
                           VALUES ('".$name."','".$commission."','".$company_id."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    $stmt = $dbh->prepare("UPDATE `booker` SET

                           `name` = '".$name."',
                           `commission` = '".$commission."',
                           `company_id` = '".$company_id."'

                           WHERE `booker`.`id_booker` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `booker` WHERE `booker`.`id_booker` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'booker id=".$post_id."'
             )");
$stmt -> execute();

header('location: booker.php');
exit;

?>

