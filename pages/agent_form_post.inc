<?php

$post_id = $_POST['id'];
$name = $_POST['name'];
$percentage = $_POST['percentage'];





if($_POST['source_action']=='add' and $_SESSION['permission']=='rw')
    {
    $stmt = $dbh->prepare("INSERT INTO `agent`
                                  (`name`,`percentage`)
                           VALUES ('".$name."','".$percentage."')
                           ");
    $stmt -> execute();
    echo '<p>Data add success</p>';

    }
elseif($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    $stmt = $dbh->prepare("UPDATE `personel` SET

                           `name` = '".$name."',
                           `percentage` = '".$percentage."'

                           WHERE `agent`.`id_agent` = '".$post_id."'
                           ");

    $stmt -> execute();
    echo '<p>Data change success</p>';
    }
elseif($_GET['action']=='del' and $_SESSION['permission']=='rw')
    {
    $post_id = $_GET['id'];


    $stmt = $dbh->prepare("DELETE FROM `agent` WHERE `agent`.`id_agent` = '".$post_id."' ");

    $stmt -> execute();
    echo '<p>Data delete success</p>';
    }

$stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
             VALUES (
             '".$_SESSION['user_id']."',
             '".$_SERVER['REMOTE_ADDR']."',
             '".$_POST['source_action']."',
             'agent id=".$post_id."'
             )");
$stmt -> execute();

header('location: agent.php');
exit;

?>

