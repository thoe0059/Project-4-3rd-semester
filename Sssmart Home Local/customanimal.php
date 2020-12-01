<!DOCTYPE HTML>

<html>
						<?php

						$first_name = $_POST['species'];
						$last_name = $_POST['humlow'];

						echo $first_name;
						echo $last_name; ?>
						</header>
					</div>
				</article>
</section>



		<!-- Scripts -->
			<script type="text/javascript" src="{{ url_for('static', filename='/js/jquery.min.js') }}"></script>
			<script type="text/javascript" src="{{ url_for('static', filename='/js/jquery.scrollex.min.js') }}"></script>
			<script type="text/javascript" src="{{ url_for('static', filename='/js/skel.min.js') }}"></script>
			<script type="text/javascript" src="{{ url_for('static', filename='/js/util.js') }}"></script>
			<script type="text/javascript" src="{{ url_for('static', filename='/js/main.js') }}"></script>
			<!-- <script type="text/javascript">
             window.onload = function(){
                var name = prompt("Please enter your animal's name");
                var nameElement = document.getElementById("name");
                nameElement.innerText = name;
            } </script> -->

	</body>
</html>