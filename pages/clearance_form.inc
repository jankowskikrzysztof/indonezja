<?php

$id = $_GET['id'];
$date_now = date("Y-m-d");

if($action=='edit')
                   {

                   $stmt = $dbh->prepare("SELECT date_from, date_to, txt_route, id_boat_trip,clearance_scan_org_filename_ext,clearance_scan_org_filename



                              FROM boat_trip
                              WHERE boat_trip.id_boat_trip='".$id."'
                              ");
                   $stmt -> execute();
                   $edit_array = $stmt->fetchAll();

                   $date_from = $edit_array[0]['date_from'];
                   $date_to = $edit_array[0]['date_to'];
                   $txt_route = $edit_array[0]['txt_route'];
                   $id_boat_trip = $edit_array[0]['id_boat_trip'];

                   $clearance_scan_org_filename_ext = $edit_array[0]['clearance_scan_org_filename_ext'];

                   $lastchange = $edit_array[0]['lastchange'];

                   }



if($_GET['datt']==1 and $_SESSION['permission']=='rw')
    {
    rename('img_uploads/boat_trip/clearance_list_'.$id.'.'.$clearance_scan_org_filename_ext,'img_uploads/boat_trip/clearance_list_'.$id.'_'.uniqid().'.'.$clearance_scan_org_filename_ext);
    }


?>



<div>

<form class="pure-form pure-form-aligned" action="?action=post" method="POST" enctype="multipart/form-data">
        <fieldset>

<input type="hidden" name="id" value="<?php echo $id ?>" >
<input type="hidden" name="source_action" value="<?php echo $action ?>" >
<input type="hidden" name="type" value="2" >



            <div class="pure-control-group">
                <label for="location_id">Empress trip</label>
                <?php

                        echo $date_from.' - '.$date_to.': '.$txt_route;

                ?>
            </div>





<?php 

if($action=='edit')
    { 
    ?>

  <div class="pure-control-group">
                <label for="price">Document scan</label>
                
<?php

    if(file_exists('img_uploads/boat_trip/clearance_list_'.$id.'.'.$clearance_scan_org_filename_ext.''))
        {
        if(in_array($scan_org_filename_ext,array('jpg','jpeg','png')))
            $img_file = '<img src="img_uploads/boat_trip/clearance_list_'.$id.'.'.$clearance_scan_org_filename_ext.'" style="max-width:200px">';
        else
            $img_file = '<i class="far fa-file-pdf fa-5x"></i>';

        echo '<a href="img_uploads/boat_trip/clearance_list_'.$id.'.'.$clearance_scan_org_filename_ext.'" target=_blank>'.$img_file.'</a> &nbsp;&nbsp;&nbsp;
        <a href="?action=edit&id='.$id.'&datt=1"><i class="fas fa-times"></i></a>';
        }
    else
        {
        echo '<input type="file" name="fileToUpload" id="fileToUpload">';
        }

    echo '</div>';


    }; ?>


            <div class="pure-controls">




                <?php
                if($_SESSION['permission']=='rw')
                   {
                   echo '<button type="submit" class="pure-button pure-button-primary">Submit</button>';

                   }


                 ?>

            </div>
        </fieldset>
    </form>