<?php

$post_id = $_POST[id];
$activity_id = $_POST[activity_id];
$location_groups_id = $_POST[location_groups_id];
$price = $_POST[price];




if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `activity_price_list`
                                  (`activity_id`,`location_groups_id`,`price`)
                           VALUES ('".$activity_id."','".$location_groups_id."','".$price."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    $stmt = $dbh->prepare("UPDATE `activity_price_list` SET

                           `activity_id` = '".$activity_id."',
                           `location_groups_id` = '".$location_groups_id."',
                           `price` = '".$price."'


                           WHERE `activity_price_list`.`id_activity_price_list` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `activity_price_list` WHERE `activity_price_list`.`id_activity_price_list` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'activities_pricelist id=".$post_id."'
             )");
$stmt -> execute();

?>

