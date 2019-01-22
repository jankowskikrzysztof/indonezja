<form id="formLogin" method="post" name="myform">
   <label>User Name:</label>
   <input type="text" name="bill_no" id="bill_no">


   <input type="submit" value="Login" id="submit">
   <input type="reset" value="Reset">
</form>

<!--<script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.2/jquery.min.js"></script>-->
<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script>
$('#formLogin').submit(function(e) {
    e.preventDefault();
    var bill_no = $('input#bill_no').val();

    if(bill_no == ""){
       alert("Please enter a Bill NO");
       $('#bill_no').focus();
       return false;
    }

    if(bill_no != '') {
        $.ajax({
            url: 'test_hash_db.php',
            type: 'POST',
            data: {
              bill_no: bill_no
            },
            success: function(data) {
                console.log(data);
                // It looks like the page that handles the form returns JSON
                // Parse the JSON
                var obj = JSON.parse(data);

                if(obj.result != 'invalid') {
                    alert("Login succeeded");
                    // You should redirect the user too
                    window.location = 'http://redirecturl.com';
                }                    
            }
        });
    } 
}); 
</script>