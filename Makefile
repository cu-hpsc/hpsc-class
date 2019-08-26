build:
	Rscript -e "blogdown::build_site()"

deploy: build
	git -C public add --all
	git -C public commit -m"built site $$(date -Iseconds)"
	git -C public push

serve:
	Rscript -e "blogdown::serve_site()"
