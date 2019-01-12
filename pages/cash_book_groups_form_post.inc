<?php

$post_id = $_POST[id];
$name = $_POST[name];
$type = $_POST[type];




if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `cash_book_group`
                                  (`name`, `type`)
                           VALUES ('".$name."', $type)
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {

    $stmt = $dbh->prepare("UPDATE `cash_book_group` SET

                           `name` = '".$name."',
                           `type` = '".$type."'

                           WHERE `cash_book_group`.`id_cash_book_group` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `cash_book_group` WHERE `cash_book_group`.`id_cash_book_group` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'cash_book_group id=".$post_id."'
             )");
$stmt -> execute();

?>

