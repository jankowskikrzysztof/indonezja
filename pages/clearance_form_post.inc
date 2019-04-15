<?php


$post_id = $_POST['id'];

$target_dir = "img_uploads/boat_trip/";
$target_file = $target_dir . 'boat_trip_' .$_POST['id'];
$uploadOk = 1;


if($_POST['source_action']=='edit' and $_SESSION['permission']=='rw')
    {


    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
        echo "Sorry, your file was not uploaded.";
    // if everything is ok, try to upload file
    } else {
        
        $file_ext = pathinfo(parse_url($_FILES["fileToUpload"]["name"])['path'], PATHINFO_EXTENSION);
        if($file_ext <> "")
            $file_info_sql = '
                            `clearance_scan_org_filename` = "'.$_FILES['fileToUpload']['name'].'",
                            `clearance_scan_org_filename_ext` = "'.$file_ext.'"';
        else
            $file_info_sql = '';

        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file.'.'.$file_ext)) {
            echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.";
            
        } elseif($_FILES["fileToUpload"]<>NULL) {
            echo "<font color=red>Sorry, there was an error uploading your file.</font>";
        }
    }


    $stmt = $dbh->prepare("UPDATE `boat_trip` SET

                           ".$file_info_sql."
                           
                           

                           WHERE `boat_trip`.`id_boat_trip` = '".$post_id."'
                           ");


    $stmt -> execute();
    echo '<p>Data change success</p>';
    }


$target_table = 'boat_trip';
$txt = 'clearance_list_scan';



    $target_id = $post_id;
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


header('location: boat_trip.php');
exit;

?>
