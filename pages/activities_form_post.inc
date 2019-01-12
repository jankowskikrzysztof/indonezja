<?php

$post_id = $_POST[id];
$shortcut = $_POST[shortcut];
$name = $_POST[name];
$description = $_POST[description];





if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `activity`
                                  (`shortcut`,`name`,`description`)
                           VALUES ('".$shortcut."','".$name."','".$description."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    $stmt = $dbh->prepare("UPDATE `activity` SET

                           `shortcut` = '".$shortcut."',
                           `name` = '".$name."',
                           `description` = '".$description."'

                           WHERE `activity`.`id_activity` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];

    $stmt = $dbh->prepare("DELETE FROM `activity` WHERE `activity`.`id_activity` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'activities id=".$post_id."'
             )");
$stmt -> execute();

header('location: activities.php');
exit;


?>

