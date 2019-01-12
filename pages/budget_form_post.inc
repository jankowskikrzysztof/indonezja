<?php

$post_id = $_POST[id];
$date = $_POST[period];
$location_groups_id = $_POST[location_groups_id];
$cash_book_group_id = $_POST[cash_book_group_id];
$value = str_replace(',','',$_POST[value]);
$type = $_POST[type];


$amount = $_POST[amount];
$activity_id = $_POST[activity_id];




if(($_POST['source_action']=='add' or $_POST['source_action']=='add_ba') and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `budget`
                                  (`cash_book_group_id`, `location_groups_id`, `date`, `value`, `amount`, `activity_id`, `type`)
                           VALUES ($cash_book_group_id, $location_groups_id, '".$date."', '".$value."', '".$amount."', '".$activity_id."', '".$type."')
                           ");

    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif(($_POST['source_action']=='edit' or $_POST['source_action']=='edit_ba') and $_SESSION['permission']=='rw')
    {

    $stmt = $dbh->prepare("UPDATE `budget` SET

                           `cash_book_group_id` = '".$cash_book_group_id."',
                           `location_groups_id` = '".$location_groups_id."',
                           `date` = '".$date."',
                           `value` = '".$value."',
                           `amount` = '".$amount."',
                           `activity_id` = '".$activity_id."'

                           WHERE `budget`.`id_budget` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif(($_GET['action']=='del' or $_GET['action']=='del_ba') and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `budget` WHERE `budget`.`id_budget` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'budget id=".$post_id."'
             )");
$stmt -> execute();

header('location: budget.php');
exit;

?>

