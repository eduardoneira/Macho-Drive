from server_test_func import *
from hashlib import md5

gabo = 'j'
pass_gabo = md5('j').hexdigest()
file_gabo = 'test_gabo.txt'
file_edu = 'test_edu.txt'

edu = 'f'
pass_edu = md5('f').hexdigest()

create_user(gabo, pass_gabo)
create_user(edu, pass_edu)

token_gabo = log_in(gabo, pass_gabo)
token_edu = log_in(edu, pass_edu)

subir_archivo(gabo, token_gabo, file_gabo, [], [], [], '')
subir_archivo(edu, token_edu, file_edu, [], [], [], '')

file_change_content_from_string(gabo, token_gabo, gabo, file_gabo, "cambio contenido", 'chile')

log_out(gabo, token_gabo)
log_out(edu, token_edu)